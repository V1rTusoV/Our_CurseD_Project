//#include "FileWork.h"
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>
#include "game.h"
#include "FileWork.h"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <fstream>

namespace file {

    void FileManager::to_json(json& j, const game::Enemy& e) {
        j = json{
            {"id", e.id},
            {"codifier", e.codifier},
            {"name", e.name},
            {"description", e.description},
            {"level", e.level},
            {"initiative", e.initiative},
            {"hp", e.hp},
            {"maxhp", e.maxhp},          // добавлено
            {"moves", e.moves},
            {"maxmoves", e.maxmoves},    // добавлено
            {"enableCards", e.enableCards},
            {"hand", e.hand},
            {"mechresist", e.mechresist},
            {"physresist", e.physresist},
            {"splashresist", e.splashresist}
        };
    }
    void FileManager::from_json(const json& j, game::Enemy& e) {
        j.at("id").get_to(e.id);
        j.at("codifier").get_to(e.codifier);
        j.at("name").get_to(e.name);
        j.at("description").get_to(e.description);
        j.at("level").get_to(e.level);
        j.at("initiative").get_to(e.initiative);
        j.at("hp").get_to(e.hp);
        if (j.contains("maxhp")) j.at("maxhp").get_to(e.maxhp);        // добавлено
        j.at("moves").get_to(e.moves);
        if (j.contains("maxmoves")) j.at("maxmoves").get_to(e.maxmoves); // добавлено
        j.at("enableCards").get_to(e.enableCards);
        j.at("hand").get_to(e.hand);
        j.at("mechresist").get_to(e.mechresist);
        j.at("physresist").get_to(e.physresist);
        j.at("splashresist").get_to(e.splashresist);
    }
    void FileManager::test() {
        game::Enemy enemy(101, "E01", "Goblin", "Small green creature", 2, 12, 45, 3);
        enemy.enableCards = { 5, 7, 9 };
        enemy.hand = { 1, 3 };
        json j;
        to_json(j, enemy);
        std::string serialized = j.dump(4, ' ',false); // форматированный вывод
        std::cout << serialized << std::endl;
    }

    void FileManager::to_json(json& j, const std::vector<game::Enemy>& vec) {
        j = json::array();                     // создаём пустой JSON-массив
        for (const auto& e : vec) {
            json item;
            to_json(item, e);                  // используем перегрузку для одного Enemy
            j.push_back(item);
        }
    }

    void FileManager::from_json(const json& j, std::vector<game::Enemy>& vec) {
        if (!j.is_array()) {
            throw std::runtime_error("JSON не является массивом");
        }
        vec.clear();
        vec.reserve(j.size());
        for (const auto& item : j) {
            game::Enemy e;
            from_json(item, e);                // используем перегрузку для одного Enemy
            vec.push_back(std::move(e));
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
            {"damage", c.damage}
        };
    }

    void FileManager::from_json(const json& j, game::CardItem& c) {
        j.at("id").get_to(c.id);
        j.at("codifier").get_to(c.codifier);
        j.at("name").get_to(c.name);
        j.at("description").get_to(c.description);
        j.at("level").get_to(c.level);
        j.at("cost").get_to(c.cost);
        std::string dtypeStr;
        j.at("dtype").get_to(dtypeStr);
        c.dtype = stringToDamageType(dtypeStr);
        j.at("damage").get_to(c.damage);
    }

    void FileManager::to_json(json& j, const std::vector<game::CardItem>& vec) {
        j = json::array();                     // создаём пустой JSON-массив
        for (const auto& e : vec) {
            json item;
            to_json(item, e);                  // используем перегрузку для одного Enemy
            j.push_back(item);
        }
    }

    void FileManager::from_json(const json& j, std::vector<game::CardItem>& vec) {
        if (!j.is_array()) {
            throw std::runtime_error("JSON не является массивом");
        }
        vec.clear();
        vec.reserve(j.size());
        for (const auto& item : j) {
            game::CardItem e;
            from_json(item, e);                // используем перегрузку для одного Enemy
            vec.push_back(std::move(e));
        }
    }

    // Пример использования этих перегрузок для записи в файл
    void FileManager::writeEnemiesToFile(const std::string& filename, const std::vector<game::Enemy>& enemies) {
        try {
            json j;
            to_json(j, enemies);   // или j = enemies;

            std::ofstream out(filename, std::ios::binary);
            if (!out.is_open()) {
                std::cerr << "4. Ошибка: не удалось открыть файл " << filename << std::endl;
                throw std::runtime_error("Не удалось открыть файл");
            }
            std::cout << "4. Файл открыт, запись..." << std::endl;

            out << j.dump(4, ' ', false);
            out.close();
            std::cout << "5. Запись завершена" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Исключение: " << e.what() << std::endl;
        }
    }

    std::vector<game::Enemy> FileManager::readEnemiesFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Не удалось открыть файл");
        json j;
        in >> j;
        std::vector<game::Enemy> result;
        from_json(j, result);   // вызов перегрузки для вектора
        return result;
    }

    void FileManager::writeCardsToFile(const std::string& filename, const std::vector<game::CardItem>& cards) {
        json j;
        to_json(j, cards);   // ручная перегрузка для вектора
        std::ofstream out(filename, std::ios::binary);
        if (!out) throw std::runtime_error("Не удалось открыть файл для записи карт");
        out << j.dump(4, ' ', false);
    }

    std::vector<game::CardItem> FileManager::readCardsFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Не удалось открыть файл для чтения карт");
        json j;
        in >> j;
        std::vector<game::CardItem> result;
        from_json(j, result);   // ручная перегрузка для вектора
        return result;
    }

    std::string FileManager::damageTypeToString(game::DamageType dt) {
        switch (dt) {
        case game::DamageType::physycs:       return "physycs";
        case game::DamageType::heal:          return "heal";
        case game::DamageType::mechanics:     return "mechanics";
        case game::DamageType::unreal:        return "unreal";
        case game::DamageType::physycs_splash: return "physycs_splash";
        case game::DamageType::summon:        return "summon";
        case game::DamageType::specific:      return "specific";
        default:                        return "unknown";
        }
    }

    game::DamageType FileManager::stringToDamageType(const std::string& str) {
        if (str == "physycs") return game::DamageType::physycs;
        if (str == "heal") return game::DamageType::heal;
        if (str == "mechanics") return game::DamageType::mechanics;
        if (str == "unreal") return game::DamageType::unreal;
        if (str == "physycs_splash") return game::DamageType::physycs_splash;
        if (str == "summon") return game::DamageType::summon;
        if (str == "specific") return game::DamageType::specific;
        throw std::runtime_error("Unknown DamageType: " + str);
    }

}