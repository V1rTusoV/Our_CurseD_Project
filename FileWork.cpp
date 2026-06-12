#include "game.h"
#include "FileWork.h"
#include <fstream>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h> // Для MultiByteToWideChar и WideCharToMultiByte
#include <nlohmann/json.hpp>

namespace file {

    std::string utf8_to_cp1251(const std::string& utf8_str) {
        if (utf8_str.empty()) return {};

        // 1. Конвертируем UTF-8 в UTF-16
        int wch_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.data(), -1, nullptr, 0);
        std::vector<wchar_t> wide_str(wch_len);
        MultiByteToWideChar(CP_UTF8, 0, utf8_str.data(), -1, wide_str.data(), wch_len);

        // 2. Конвертируем UTF-16 в CP1251
        int cp_len = WideCharToMultiByte(1251, 0, wide_str.data(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> cp1251_str(cp_len);
        WideCharToMultiByte(1251, 0, wide_str.data(), -1, cp1251_str.data(), cp_len, nullptr, nullptr);

        return cp1251_str.data();
    }

    //serialiaze block
    void FileManager::to_json(json& j, const game::Enemy& e) {
        j = json{
            {"id", e.id},
            {"codifier", e.codifier},
            {"name", e.name},
            {"description", e.description},
            {"level", e.level},
            {"initiative", e.initiative},
            {"hp", e.hp},
            {"maxhp", e.maxhp},
            {"moves", e.moves},
            {"maxmoves", e.maxmoves},
            {"gainexp", e.gainexp},
            {"gainmoney", e.gainmoney},
            {"enableCards", e.enableCards},
            {"hand", e.hand},
            {"mechresist", e.mechresist},
            {"physresist", e.physresist},
            {"splashresist", e.splashresist},
            {"img", e.img}
        };
    }
    void FileManager::to_json(json& j, const std::vector<game::Enemy>& vec) {
        j = json::array();
        for (const auto& e : vec) {
            json item;
            to_json(item, e);
            j.push_back(item);
        }
    }
    void FileManager::to_json(json& j, const game::CardItem& c) {
        j = json{
            {"id", c.id},
            {"codifier", c.codifier},
            {"name", c.name},
            {"description", c.description},
            {"level", c.level},
            {"cost", c.cost},
            {"dtype", damageTypeToString(c.dtype)},
            {"damage", c.damage},
            {"moneycost", c.moneycost},
            {"img", c.img}
        };
    }
    void FileManager::to_json(json& j, const std::vector<game::CardItem>& vec) {
        j = json::array();
        for (const auto& e : vec) {
            json item;
            to_json(item, e);
            j.push_back(item);
        }
    }
    void FileManager::to_json(json& j, const game::Event& e) {
        j = json{
            {"id", e.id},
            {"codifier", e.codifier},
            {"description", e.description},
            {"type", eventTypeToString(e.type)},
            {"value1", e.value1},
            {"value2", e.value2}
        };
    }
    void FileManager::to_json(json& j, const std::vector<game::Event>& vec) {
        j = json::array();
        for (const auto& e : vec) {
            json item;
            to_json(item, e);
            j.push_back(item);
        }
    }

    //deserializ block
    void FileManager::from_json(const json& j, game::Enemy& e) {
        j.at("id").get_to(e.id);
        j.at("codifier").get_to(e.codifier);
        j.at("name").get_to(e.name);
        j.at("description").get_to(e.description);
        j.at("level").get_to(e.level);
        j.at("initiative").get_to(e.initiative);
        j.at("hp").get_to(e.hp);
        j.at("enableCards").get_to(e.enableCards);
        j.at("hand").get_to(e.hand);

        //For new Polya enemy
        e.maxhp = j.value("maxhp", e.hp);
        e.moves = j.value("moves", 2);
        e.maxmoves = j.value("maxmoves", e.moves);
        e.gainexp = j.value("gainexp", 10);
        e.gainmoney = j.value("gainmoney", 10);
        e.mechresist = j.value("mechresist", 0);
        e.physresist = j.value("physresist", 0);
        e.splashresist = j.value("splashresist", 0);
        e.img = j.value("img", "temp");

        e.name = utf8_to_cp1251(e.name);
        e.description = utf8_to_cp1251(e.description);
    }
    void FileManager::from_json(const json& j, std::vector<game::Enemy>& vec) {
        if (!j.is_array()) {
            throw std::runtime_error("JSON не является массивом");
        }
        vec.clear();
        vec.reserve(j.size());
        for (const auto& item : j) {
            game::Enemy e;
            from_json(item, e);
            vec.push_back(std::move(e));
        }
    }
    void FileManager::from_json(const json& j, game::CardItem& c) {
        j.at("id").get_to(c.id);
        j.at("codifier").get_to(c.codifier);
        j.at("name").get_to(c.name);
        j.at("description").get_to(c.description);
        j.at("level").get_to(c.level);
        j.at("cost").get_to(c.cost);
        j.at("damage").get_to(c.damage);
        j.at("moneycost").get_to(c.moneycost);

        std::string dtypeStr;
        j.at("dtype").get_to(dtypeStr);
        c.dtype = stringToDamageType(dtypeStr);
        c.img = j.value("img", "temp");

        c.name = utf8_to_cp1251(c.name);
        c.description = utf8_to_cp1251(c.description);
    }
    void FileManager::from_json(const json& j, std::vector<game::CardItem>& vec) {
        if (!j.is_array()) {
            throw std::runtime_error("JSON не является массивом");
        }
        vec.clear();
        vec.reserve(j.size());
        for (const auto& item : j) {
            game::CardItem c;
            from_json(item, c);
            vec.push_back(std::move(c));
        }
    }
    void FileManager::from_json(const json& j, game::Event& e) {
        j.at("id").get_to(e.id);
        j.at("codifier").get_to(e.codifier);
        j.at("description").get_to(e.description);
        j.at("value1").get_to(e.value1);
        j.at("value2").get_to(e.value2);

        std::string typeStr;
        j.at("type").get_to(typeStr);
        e.type = stringToEventType(typeStr);

        e.description = utf8_to_cp1251(e.description);
    }
    void FileManager::from_json(const json& j, std::vector<game::Event>& vec) {
        if (!j.is_array()) {
            throw std::runtime_error("JSON не является массивом");
        }
        vec.clear();
        vec.reserve(j.size());
        for (const auto& item : j) {
            game::Event e;
            from_json(item, e);
            vec.push_back(std::move(e));
        }
    }

    //Write block
    void FileManager::writeEnemiesToFile(const std::string& filename, const std::vector<game::Enemy>& enemies) {
        json j;
        to_json(j, enemies);
        std::ofstream out(filename, std::ios::binary);
        if (!out.is_open()) throw std::runtime_error("Не удалось открыть файл для записи врагов");
        out << j.dump(4, ' ', false);
        out.close();
    }
    void FileManager::writeCardsToFile(const std::string& filename, const std::vector<game::CardItem>& cards) {
        json j;
        to_json(j, cards);
        std::ofstream out(filename, std::ios::binary);
        if (!out) throw std::runtime_error("Не удалось открыть файл для записи карт");
        out << j.dump(4, ' ', false);
        out.close();
    }
    void FileManager::writeEventsToFile(const std::string& filename, const std::vector<game::Event>& events) {
        json j;
        to_json(j, events);
        std::ofstream out(filename, std::ios::binary);
        if (!out) throw std::runtime_error("Не удалось открыть файл для записи событий");
        out << j.dump(4, ' ', false);
        out.close();
    }

    //Read block
    std::vector<game::Enemy> FileManager::readEnemiesFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Не удалось открыть файл для чтения врагов");
        json j;
        in >> j;
        std::vector<game::Enemy> result;
        from_json(j, result);
        return result;
    }
    std::vector<game::CardItem> FileManager::readCardsFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Не удалось открыть файл для чтения карт");
        json j;
        in >> j;
        std::vector<game::CardItem> result;
        from_json(j, result);
        return result;
    }
    std::vector<game::Event> FileManager::readEventsFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Не удалось открыть файл для чтения событий");
        json j;
        in >> j;
        std::vector<game::Event> result;
        from_json(j, result);
        return result;
    }

    //Sub functions

    std::string FileManager::damageTypeToString(game::DamageType dt) {
        switch (dt) {
        case game::DamageType::physycs:        return "physycs";
        case game::DamageType::heal:           return "heal";
        case game::DamageType::mechanics:      return "mechanics";
        case game::DamageType::unreal:         return "unreal";
        case game::DamageType::physycs_splash: return "physycs_splash";
        case game::DamageType::summon:         return "summon";
        case game::DamageType::specific:       return "specific";
        default:                               return "unknown";
        }
    }
    game::DamageType FileManager::stringToDamageType(const std::string& str) {
        if (str == "physycs")        return game::DamageType::physycs;
        if (str == "heal")           return game::DamageType::heal;
        if (str == "mechanics")      return game::DamageType::mechanics;
        if (str == "unreal")         return game::DamageType::unreal;
        if (str == "physycs_splash") return game::DamageType::physycs_splash;
        if (str == "summon")         return game::DamageType::summon;
        if (str == "specific")       return game::DamageType::specific;
        throw std::runtime_error("Unknown DamageType: " + str);
    }

    std::string FileManager::eventTypeToString(game::EventType et) {
        switch (et) {
        case game::AddItem:      return "AddItem";
        case game::RemoveItem:   return "RemoveItem";
        case game::AddResist:    return "AddResist";
        case game::RemoveResist: return "RemoveResist";
        case game::AddMoney:     return "AddMoney";
        case game::RemoveMoney:  return "RemoveMoney";
        case game::AddMoves:     return "AddMoves";
        case game::RemoveMoves:  return "RemoveMoves";
        case game::AddMaxHp:     return "AddMaxHp";
        case game::RemoveMaxHp:  return "RemoveMaxHp";
        case game::AddHp:        return "AddHp";
        case game::RemoveHp:     return "RemoveHp";
        default:                 return "AddItem";
        }
    }
    game::EventType FileManager::stringToEventType(const std::string& str) {
        if (str == "AddItem")      return game::AddItem;
        if (str == "RemoveItem")   return game::RemoveItem;
        if (str == "AddResist")    return game::AddResist;
        if (str == "RemoveResist") return game::RemoveResist;
        if (str == "AddMoney")     return game::AddMoney;
        if (str == "RemoveMoney")  return game::RemoveMoney;
        if (str == "AddMoves")     return game::AddMoves;
        if (str == "RemoveMoves")  return game::RemoveMoves;
        if (str == "AddMaxHp")     return game::AddMaxHp;
        if (str == "RemoveMaxHp")  return game::RemoveMaxHp;
        if (str == "AddHp")        return game::AddHp;
        if (str == "RemoveHp")     return game::RemoveHp;
        return game::AddItem;
    }

} // namespace file