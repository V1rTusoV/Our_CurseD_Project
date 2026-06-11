//Include block
#include "game.h"
#include "FileWork.h"

//game namespace
namespace game {

    //init functions
    void GameObject::initapp() {
        totalvolume = 1.0f;
        musicvolume = 1.0f;
        effectvolume = 1.0f;
        soundvolume = 1.0f;
    }
    void GameObject::initgame() {
        srand(static_cast<unsigned>(time(nullptr)));
        seed = rand();
        currentFloor = 1;
        runstart = time(nullptr);
        currentNodeId = 0;
        map = GenerateGraph();

        file::FileManager fm;
        enemys = fm.readEnemiesFromFile("enemies.json");
        cards = fm.readCardsFromFile("cards.json");
        events = fm.readEventsFromFile("events.json");
    }

    //main functions
    void GameObject::play() {
        initgame();
        bool flag = true;
        int gcode = 0;

        file::FileManager fm;

        while (flag) {
            ShowPlayerStats();
            gcode = MapSegment();
            switch (gcode) {
            case 1:
                if (BattleSegment() == 0) flag = 0;
                break;
            case 2:
                ShopSegment();
                break;
            case 3:
                SpecialSegment();
                break;
            case 4:
                currentNodeId = 0;
                map = GenerateGraph();
                break;
            case 404:
                std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                std::cout << "current floor:" << currentFloor << std::endl;
                std::cout << "current flag:" << flag << std::endl;
                break;
            }
            if (gcode == 0 || gcode == 5) flag = 0;
        }
    }
    void GameObject::applyEvent(const Event& e) {
        switch (e.type) {
        case AddItem: {
            player.getEnableCards().push_back(e.value1);
            std::string cardName = "неизвестная карта";
            for (const auto& c : cards) {
                if (c.id == e.value1) {
                    cardName = c.name;
                    break;
                }
            }
            std::cout << COLOR_GREEN << "+ Добавлена карта: " << cardName << COLOR_RESET << "\n";
            break;
        }
        case RemoveItem: {
            if (!player.getEnableCards().empty()) {
                int idx = rand() % player.getEnableCards().size();
                int removedId = player.getEnableCards()[idx];
                std::string cardName = "неизвестная карта";
                for (const auto& c : cards) {
                    if (c.id == removedId) {
                        cardName = c.name;
                        break;
                    }
                }
                player.getEnableCards().erase(player.getEnableCards().begin() + idx);
                std::cout << COLOR_RED << "- Удалена карта: " << cardName << COLOR_RESET << "\n";
            }
            else {
                std::cout << "Нет карт для удаления.\n";
            }
            break;
        }
        case AddResist: {
            if (e.value1 == 0) {
                player.addMechresist(e.value2);
                std::cout << COLOR_GREEN << "+ Мехresist увеличен на " << e.value2 << " (теперь " << player.getMechresist() << ")" << COLOR_RESET << "\n";
            }
            else if (e.value1 == 1) {
                player.addPhysresist(e.value2);
                std::cout << COLOR_GREEN << "+ Физresist увеличен на " << e.value2 << " (теперь " << player.getPhysresist() << ")" << COLOR_RESET << "\n";
            }
            else if (e.value1 == 2) {
                player.addSplashresist(e.value2);
                std::cout << COLOR_GREEN << "+ Сплешresist увеличен на " << e.value2 << " (теперь " << player.getSplashresist() << ")" << COLOR_RESET << "\n";
            }
            break;
        }
        case RemoveResist: {
            if (e.value1 == 0) {
                player.addMechresist(-e.value2);
                std::cout << COLOR_RED << "- Мехresist уменьшен на " << e.value2 << " (теперь " << player.getMechresist() << ")" << COLOR_RESET << "\n";
            }
            else if (e.value1 == 1) {
                player.addPhysresist(-e.value2);
                std::cout << COLOR_RED << "- Физresist уменьшен на " << e.value2 << " (теперь " << player.getPhysresist() << ")" << COLOR_RESET << "\n";
            }
            else if (e.value1 == 2) {
                player.addSplashresist(-e.value2);
                std::cout << COLOR_RED << "- Сплешresist уменьшен на " << e.value2 << " (теперь " << player.getSplashresist() << ")" << COLOR_RESET << "\n";
            }
            break;
        }
        case AddMoney: {
            player.addMoney(e.value1);
            std::cout << COLOR_GREEN << "+ " << e.value1 << " фишек" << COLOR_RESET << "\n";
            break;
        }
        case RemoveMoney: {
            player.addMoney(-e.value1);
            std::cout << COLOR_RED << "- " << e.value1 << " фишек" << COLOR_RESET << "\n";
            break;
        }
        case AddMoves: {
            player.addMaxMoves(e.value1);
            std::cout << COLOR_GREEN << "+ Максимальное количество ходов увеличено на " << e.value1 << " (теперь " << player.getMaxMoves() << ")" << COLOR_RESET << "\n";
            break;
        }
        case RemoveMoves: {
            player.addMaxMoves(-e.value1);
            std::cout << COLOR_RED << "- Максимальное количество ходов уменьшено на " << e.value1 << " (теперь " << player.getMaxMoves() << ")" << COLOR_RESET << "\n";
            break;
        }
        case AddMaxHp: {
            player.addMaxHp(e.value1);
            std::cout << COLOR_GREEN << "+ Максимальное HP увеличено на " << e.value1 << " (теперь " << player.getMaxHp() << ")" << COLOR_RESET << "\n";
            break;
        }
        case RemoveMaxHp: {
            player.addMaxHp(-e.value1);
            std::cout << COLOR_RED << "- Максимальное HP уменьшено на " << e.value1 << " (теперь " << player.getMaxHp() << ")" << COLOR_RESET << "\n";
            break;
        }
        case AddHp: {
            int oldHp = player.getHp();
            player.setHp(oldHp + e.value1);
            int healed = player.getHp() - oldHp;
            std::cout << COLOR_GREEN << "+ Восстановлено " << healed << " HP (теперь " << player.getHp() << "/" << player.getMaxHp() << ")" << COLOR_RESET << "\n";
            break;
        }
        case RemoveHp: {
            int oldHp = player.getHp();
            int newHp = oldHp - e.value1;
            if (newHp < 1) newHp = 1;
            player.setHp(newHp);
            int damage = oldHp - player.getHp();
            std::cout << COLOR_RED << "- Получено " << damage << " урона (теперь " << player.getHp() << "/" << player.getMaxHp() << ")" << COLOR_RESET << "\n";
            break;
        }
        default:
            std::cout << "Неизвестный тип события.\n";
            break;
        }
    }

    //show functions
    void GameObject::ShowPlayerStats() {
        system("cls");
        const int WIDTH = 80;
        printSeparator('=', WIDTH);
        std::cout << "СТАТИСТИКА ИГРОКА\n";
        printSeparator('=', WIDTH);
        std::cout << "Уровень: " << player.getLevel() << "\n";
        std::cout << "Опыт: " << player.getExp() << " / " << player.getExpForNextLevel() << "\n";
        std::cout << "Фишки (деньги): " << player.getMoney() << "\n";
        std::cout << "Доступные карты: ";

        const auto& enableCards = player.getEnableCards();
        if (enableCards.empty()) {
            std::cout << "(нет)";
        }
        else {
            std::set<std::string> uniqueNames;
            for (int id : enableCards) {
                for (const auto& c : cards) {
                    if (c.id == id) {
                        uniqueNames.insert(c.name);
                        break;
                    }
                }
            }
            bool first = true;
            for (const std::string& name : uniqueNames) {
                if (!first) std::cout << ", ";
                std::cout << name;
                first = false;
            }
        }
        std::cout << "\n";
        printSeparator('=', WIDTH);
        std::cout << "\nНажмите Enter для возврата...";
        std::cin.ignore(100, '\n');
        std::cin.get();
        system("cls");
    }
    void GameObject::printSeparator(char ch, int width) {
        for (int i = 0; i < width; ++i) std::cout << ch;
        std::cout << "\n";
    }
    void GameObject::printMainHeader() {
        printSeparator('=', SEPARATOR_WIDTH);
        std::string title = "Карты на стол господа!";
        int padding = (SEPARATOR_WIDTH - title.length()) / 2;
        std::cout << std::string(padding, ' ') << title << "\n";
        printSeparator('=', SEPARATOR_WIDTH);
    }
    void GameObject::printTopStats(const std::vector<game::Enemy>& battleEnemies,
        int playerHp, int playerMaxHp) {
        std::cout << COLOR_BLUE << "Игрок:" << COLOR_RESET << "\n";
        std::cout << COLOR_BLUE << "  Хп: " << playerHp << "/" << playerMaxHp << COLOR_RESET << "\n\n";

        for (size_t i = 0; i < battleEnemies.size(); ++i) {
            const auto& e = battleEnemies[i];
            std::cout << COLOR_RED << e.name << ":" << COLOR_RESET << "\n";
            std::cout << COLOR_RED << "  Хп: " << e.hp << "/" << e.maxhp << COLOR_RESET << "\n\n";
        }
        printSeparator('-', SEPARATOR_WIDTH);
    }
    void GameObject::printHandAndMoves(const std::vector<int>& playerHand,
        int playerMovesRemaining,
        const std::vector<game::CardItem>& cards) {
        std::cout << "Осталось ходов: " << playerMovesRemaining << "\n";
        std::cout << "Ваша рука:\n";
        if (playerHand.empty()) {
            std::cout << "(пусто)\n";
        }
        else {
            auto getCardById = [&](int id) -> game::CardItem* {
                for (auto& c : cards) if (c.id == id) return const_cast<game::CardItem*>(&c);
                return nullptr;
                };
            for (size_t i = 0; i < playerHand.size(); ++i) {
                game::CardItem* c = getCardById(playerHand[i]);
                if (c) {
                    std::string typeStr;
                    switch (c->dtype) {
                    case game::DamageType::physycs: typeStr = "Физ"; break;
                    case game::DamageType::mechanics: typeStr = "Мех"; break;
                    case game::DamageType::physycs_splash: typeStr = "Сплэш"; break;
                    case game::DamageType::heal: typeStr = "Хил"; break;
                    case game::DamageType::unreal: typeStr = "Ирреал"; break;
                    default: typeStr = "спец";
                    }
                    std::cout << "  " << i + 1 << ". " << c->name
                        << " | Тип: " << typeStr
                        << " | Стоимость: " << c->cost << "\n";
                }
            }
        }
        std::cout << "\n7 - осмотреть оппонентов\n";
        std::cout << "8 - посмотреть в карты\n";
        std::cout << "0 - холд(пропустить ход)\n";
        printSeparator('=', SEPARATOR_WIDTH);
        std::cout << "Ваш выбор: ";
    }
    void GameObject::printInspectCards(const std::vector<int>& playerHand,
        const std::vector<game::CardItem>& cards) {
        system("cls");
        printSeparator('=', 80);
        std::cout << "Ваши карты в руке:\n";
        if (playerHand.empty()) {
            std::cout << "(пусто)\n";
        }
        else {
            auto getCardById = [&](int id) -> game::CardItem* {
                for (auto& c : cards) if (c.id == id) return const_cast<game::CardItem*>(&c);
                return nullptr;
                };
            for (size_t i = 0; i < playerHand.size(); ++i) {
                game::CardItem* c = getCardById(playerHand[i]);
                if (c) {
                    std::cout << "\n--- Карта " << i + 1 << " ---\n";
                    std::cout << "Название: " << c->name << "\n";
                    std::cout << "Описание: " << c->description << "\n";
                    std::cout << "Уровень: " << c->level << "\n";
                    std::cout << "Стоимость: " << c->cost << "\n";
                    std::cout << "Тип урона: ";
                    switch (c->dtype) {
                    case game::DamageType::physycs: std::cout << "Физический"; break;
                    case game::DamageType::mechanics: std::cout << "Механический"; break;
                    case game::DamageType::physycs_splash: std::cout << "Физический (сплеш)"; break;
                    case game::DamageType::heal: std::cout << "Лечение"; break;
                    case game::DamageType::unreal: std::cout << "Нереальный"; break;
                    default: std::cout << "Специальный";
                    }
                    std::cout << "\nУрон: " << c->damage << "\n";
                }
            }
        }
        printSeparator('=', 80);
        std::cout << "\nНажмите Enter для возврата...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
        system("cls");
    }
    void GameObject::printConsequences(const std::string& message) {
        system("cls");
        printSeparator('=', SEPARATOR_WIDTH);
        std::cout << "ПОСЛЕДСТВИЯ\n";
        printSeparator('-', SEPARATOR_WIDTH);
        std::cout << message << "\n";
        printSeparator('=', SEPARATOR_WIDTH);
        std::cout << "\nЖдем подтверждения ставки от вас...(Нажмите Enter)";
        std::cin.ignore(1000, '\n');
        std::cin.get();
        system("cls");
    }
    void GameObject::printInspectMenu(const std::vector<game::Enemy>& battleEnemies) {
        system("cls");
        printSeparator('=', SEPARATOR_WIDTH);
        for (size_t i = 0; i < battleEnemies.size(); ++i) {
            const game::Enemy& e = battleEnemies[i];
            std::cout << i + 1 << " - " << e.name << "\n";
            std::cout << "   Описание: " << e.description << "\n";
            std::cout << "   Резисты: Мех=" << e.mechresist
                << " Физ=" << e.physresist
                << " Сплеш=" << e.splashresist << "\n\n";
        }
        printSeparator('=', SEPARATOR_WIDTH);
        std::cout << "\nНажмите Enter для возврата...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
        system("cls");
    }
    void GameObject::waitForEnter() {
        std::cout << "\nЖдем подтверждения ставки от вас...(Нажмите Enter)";
        std::cin.ignore(1000, '\n');
        std::cin.get();
        system("cls");
    }

    //segments functions
    int GameObject::MapSegment() {
        // 0 - Завершение игры
        // 1 - Переход в боевую сцену
        // 2 - Переход в сцену магазина
        // 3 - Переход в сцену SPECIAL
        // 4 - Переход на след. этаж

        printGraphAscii(map, currentNodeId);

        if (currentNodeId < 0 || currentNodeId >= (int)map.nodes.size()) {
            std::cout << "Ошибка: текущий узел не существует.\n";
            return 0;
        }

        const auto& nextNodes = map.nodes[currentNodeId].next;
        if (nextNodes.empty()) {
            std::cout << "Из текущего узла нет доступных переходов. Завершение игры.\n";
            return 0;
        }

        std::cout << "Доступные узлы для перехода: ";
        bool first = true;
        for (int nodeId : nextNodes) {
            if (!first) std::cout << ", ";
            const auto& node = map.nodes[nodeId];
            const char* color = COLOR_RESET;
            switch (node.type) {
            case START:   color = COLOR_GREEN;   break;
            case BATTLE:  color = COLOR_RED;     break;
            case SHOP:    color = COLOR_BLUE;    break;
            case SPECIAL: color = COLOR_YELLOW;  break;
            case BOSS:    color = COLOR_MAGENTA; break;
            default:      color = COLOR_RESET;   break;
            }
            std::cout << color << nodeId << COLOR_RESET;
            first = false;
        }
        std::cout << "\n";

        int nextNode = -1;
        while (true) {
            std::cout << "Enter your next node to travel: ";
            std::cin >> nextNode;
            if (nextNodes.find(nextNode) != nextNodes.end()) {
                break;
            }
            std::cout << "Неверный узел. Выберите один из доступных: ";
            for (int nodeId : nextNodes) {
                const auto& node = map.nodes[nodeId];
                const char* color = COLOR_RESET;
                switch (node.type) {
                case START:   color = COLOR_GREEN;   break;
                case BATTLE:  color = COLOR_RED;     break;
                case SHOP:    color = COLOR_BLUE;    break;
                case SPECIAL: color = COLOR_YELLOW;  break;
                case BOSS:    color = COLOR_MAGENTA; break;
                default:      color = COLOR_RESET;   break;
                }
                std::cout << color << nodeId << COLOR_RESET << " ";
            }
            std::cout << std::endl;
        }

        currentNodeId = nextNode;

        switch (map.nodes[currentNodeId].type) {
        case BATTLE:  return 1;
        case SHOP:    return 2;
        case SPECIAL: return 3;
        case BOSS:    currentFloor++; return 4;
        default:      return 404;
        }
    }
    int GameObject::BattleSegment() {
        // 1. Выбор противников
        std::vector<Enemy*> availableEnemies;
        for (auto& e : enemys) availableEnemies.push_back(&e);

        for (size_t i = availableEnemies.size(); i > 1; --i) {
            size_t j = rand() % i;
            std::swap(availableEnemies[i - 1], availableEnemies[j]);
        }

        int playerLevel = player.getLevel();
        int maxTotalLevel = playerLevel * 2;
        int currentTotalLevel = 0;
        std::vector<Enemy> battleEnemies;

        for (auto* e : availableEnemies) {
            if (battleEnemies.size() >= MAX_ENEMY_IN_BATTLE) break;
            if (currentTotalLevel + e->level <= maxTotalLevel) {
                battleEnemies.push_back(*e);
                currentTotalLevel += e->level;
            }
        }

        if (battleEnemies.empty()) {
            std::cout << "Нет подходящих врагов для боя!\n";
            return 1;
        }

        // 2. Локальное состояние игрока
        int playerHp = player.getHp();
        int playerMaxHp = player.getMaxHp();
        std::vector<int> playerEnableCards = player.getEnableCards();
        std::vector<int> playerHand;
        int playerMovesRemaining = 0;

        // 3. Лямбды
        auto getCardById = [&](int id) -> CardItem* {
            for (auto& c : cards) if (c.id == id) return &c;
            return nullptr;
            };

        auto drawCards = [&](std::vector<int>& hand, std::vector<int>& enableCards, int maxDraw, int maxHandSize) {
            int drawn = 0;
            while (drawn < maxDraw && hand.size() < maxHandSize && !enableCards.empty()) {
                int idx = rand() % enableCards.size();
                int cardId = enableCards[idx];
                hand.push_back(cardId);
                enableCards.erase(enableCards.begin() + idx);
                drawn++;
            }
            };

        auto returnCardToEnable = [&](std::vector<int>& hand, std::vector<int>& enableCards, int cardId) {
            auto it = std::find(hand.begin(), hand.end(), cardId);
            if (it != hand.end()) {
                hand.erase(it);
                enableCards.push_back(cardId);
            }
            };

        auto applyDamageToPlayer = [&](int damage, DamageType dtype) -> int {
            int resist = 0;
            switch (dtype) {
            case DamageType::mechanics: resist = player.getMechresist(); break;
            case DamageType::physycs:   resist = player.getPhysresist(); break;
            case DamageType::physycs_splash: resist = player.getSplashresist(); break;
            default: resist = 0; break;
            }
            int finalDamage = damage * (100 - resist) / 100;
            if (finalDamage <= 0) finalDamage = 1;
            playerHp -= finalDamage;
            if (playerHp < 0) playerHp = 0;
            return finalDamage;
            };

        auto applyHealToPlayer = [&](int amount) {
            playerHp += amount;
            if (playerHp > playerMaxHp) playerHp = playerMaxHp;
            };

        auto applyDamageToEnemy = [&](Enemy& enemy, int damage, DamageType dtype) -> int {
            int resist = 0;
            switch (dtype) {
            case DamageType::mechanics: resist = enemy.mechresist; break;
            case DamageType::physycs:   resist = enemy.physresist; break;
            case DamageType::physycs_splash: resist = enemy.splashresist; break;
            default: resist = 0; break;
            }
            int finalDamage = damage * (100 - resist) / 100;
            if (finalDamage <= 0) finalDamage = 1;
            enemy.hp -= finalDamage;
            if (enemy.hp < 0) enemy.hp = 0;
            return finalDamage;
            };

        auto applyHealToEnemy = [&](Enemy& enemy, int amount) {
            enemy.hp += amount;
            if (enemy.hp > enemy.maxhp) enemy.hp = enemy.maxhp;
            };

        // 4. Основной цикл боя
        bool playerAlive = true;
        bool enemiesAlive = true;

        while (playerAlive && enemiesAlive) {
            // Сбор по инициативе
            struct Turn { bool isPlayer; int initiative; int enemyIndex; };
            std::vector<Turn> turns;
            turns.push_back({ true, player.getInitiative(), -1 });
            for (size_t i = 0; i < battleEnemies.size(); ++i)
                if (battleEnemies[i].hp > 0)
                    turns.push_back({ false, battleEnemies[i].initiative, (int)i });
            std::sort(turns.begin(), turns.end(), [](const Turn& a, const Turn& b) {
                if (a.initiative != b.initiative) return a.initiative > b.initiative;
                return a.isPlayer;
                });

            for (const auto& turn : turns) {
                if (!playerAlive || !enemiesAlive) break;

                if (turn.isPlayer) {
                    playerMovesRemaining = player.getMaxMoves();
                    drawCards(playerHand, playerEnableCards, 2, 6);
                    bool endTurn = false;

                    while (!endTurn && playerMovesRemaining > 0 && !playerHand.empty()) {
                        printMainHeader();
                        printTopStats(battleEnemies, playerHp, playerMaxHp);
                        printHandAndMoves(playerHand, playerMovesRemaining, cards);

                        int choice = 0;
                        std::cin >> choice;
                        if (choice == 0) {
                            endTurn = true;
                            break;
                        }
                        else if (choice == 7) {
                            printInspectMenu(battleEnemies);
                            continue;
                        }
                        else if (choice == 8) {
                            printInspectCards(playerHand, cards);
                            continue;
                        }
                        else if (choice < 1 || choice >(int)playerHand.size()) {
                            std::cout << "Неверный номер.\n";
                            waitForEnter();
                            continue;
                        }

                        int cardId = playerHand[choice - 1];
                        CardItem* card = getCardById(cardId);
                        if (!card) continue;
                        if (card->cost > playerMovesRemaining) {
                            std::cout << "Недостаточно moves.\n";
                            waitForEnter();
                            continue;
                        }

                        std::string consequence;
                        if (card->dtype == DamageType::heal) {
                            int oldHp = playerHp;
                            applyHealToPlayer(card->damage);
                            consequence = "Игрок применил " + card->name + " и восстановил " + std::to_string(playerHp - oldHp) + " HP. HP теперь " + std::to_string(playerHp);
                            returnCardToEnable(playerHand, playerEnableCards, cardId);
                            playerMovesRemaining -= card->cost;
                        }
                        else {
                            // выбор цели
                            std::vector<int> alive;
                            for (size_t i = 0; i < battleEnemies.size(); ++i)
                                if (battleEnemies[i].hp > 0) alive.push_back(i);
                            if (alive.empty()) { enemiesAlive = false; break; }

                            printSeparator('=', 80);
                            for (size_t i = 0; i < alive.size(); ++i) {
                                int idx = alive[i];
                                std::cout << i + 1 << " - " << battleEnemies[idx].name
                                    << " (HP " << battleEnemies[idx].hp << "/" << battleEnemies[idx].maxhp << ")\n";
                            }
                            printSeparator('=', 80);
                            std::cout << "\nВаш выбор: ";
                            int targetChoice;
                            std::cin >> targetChoice;
                            if (targetChoice < 1 || targetChoice >(int)alive.size()) {
                                std::cout << "Неверная цель.\n";
                                waitForEnter();
                                continue;
                            }
                            Enemy& target = battleEnemies[alive[targetChoice - 1]];
                            int dealt = applyDamageToEnemy(target, card->damage, card->dtype);
                            consequence = "Игрок атаковал " + target.name + " картой " + card->name +
                                " и нанёс " + std::to_string(dealt) + " урона. HP теперь " + std::to_string(target.hp);
                            returnCardToEnable(playerHand, playerEnableCards, cardId);
                            playerMovesRemaining -= card->cost;
                            if (target.hp <= 0) consequence += ". " + target.name + " повержен!";
                        }
                        printConsequences(consequence);

                        enemiesAlive = false;
                        for (auto& e : battleEnemies) if (e.hp > 0) enemiesAlive = true;
                        if (!enemiesAlive) break;
                    }
                }
                else {
                    Enemy& enemy = battleEnemies[turn.enemyIndex];
                    if (enemy.hp <= 0) continue;

                    int enemyMoves = enemy.moves;
                    drawCards(enemy.hand, enemy.enableCards, 2, 6);
                    bool played = false;
                    std::string consequence;

                    if (enemy.hp < enemy.maxhp * 0.3) {
                        for (auto it = enemy.hand.begin(); it != enemy.hand.end(); ++it) {
                            CardItem* card = getCardById(*it);
                            if (card && card->dtype == DamageType::heal && card->cost <= enemyMoves) {
                                int oldHp = enemy.hp;
                                applyHealToEnemy(enemy, card->damage);
                                consequence = enemy.name + " применил " + card->name + " и восстановил " +
                                    std::to_string(enemy.hp - oldHp) + " HP. HP теперь " + std::to_string(enemy.hp);
                                enemy.hand.erase(it);
                                enemy.enableCards.push_back(card->id);
                                played = true;
                                break;
                            }
                        }
                    }
                    if (!played) {
                        std::vector<int> attackCards;
                        for (int cid : enemy.hand) {
                            CardItem* card = getCardById(cid);
                            if (card && card->dtype != DamageType::heal && card->cost <= enemyMoves)
                                attackCards.push_back(cid);
                        }
                        if (!attackCards.empty()) {
                            int idx = rand() % attackCards.size();
                            CardItem* card = getCardById(attackCards[idx]);
                            int dealt = applyDamageToPlayer(card->damage, card->dtype);
                            consequence = enemy.name + " атаковал игрока картой " + card->name +
                                " и нанёс " + std::to_string(dealt) + " урона. HP игрока теперь " + std::to_string(playerHp);
                            auto it = std::find(enemy.hand.begin(), enemy.hand.end(), attackCards[idx]);
                            if (it != enemy.hand.end()) enemy.hand.erase(it);
                            enemy.enableCards.push_back(card->id);
                            played = true;
                        }
                    }
                    if (!played) consequence = enemy.name + " не смог сыграть ни одной карты.";
                    printConsequences(consequence);
                    if (playerHp <= 0) playerAlive = false;
                }
            }
            if (playerHp <= 0) playerAlive = false;
            enemiesAlive = false;
            for (auto& e : battleEnemies) if (e.hp > 0) enemiesAlive = true;
        }

        // 5. Завершение боя
        if (!playerAlive) {
            printSeparator('=', SEPARATOR_WIDTH);
            std::cout << "ВЫ ПОВЕРЖЕНЫ...\n";
            printSeparator('=', SEPARATOR_WIDTH);
            waitForEnter();
            return 0;
        }
        else {
            int totalExp = 0;
            int totalMoney = 0;
            for (const auto& e : battleEnemies) {
                totalExp += e.gainexp;
                totalMoney += e.gainmoney;
            }

            player.addExp(totalExp);
            player.addMoney(totalMoney);
            player.setHp(playerHp);
            player.getEnableCards() = playerEnableCards;

            printSeparator('=', SEPARATOR_WIDTH);
            std::cout << "ПОБЕДА!\n";
            std::cout << "Получено опыта: " << totalExp << "\n";
            std::cout << "Получено денег: " << totalMoney << "\n";
            std::cout << "Текущий уровень игрока: " << player.getLevel() << "\n";
            printSeparator('=', SEPARATOR_WIDTH);
            waitForEnter();
            return 1;
        }
    }
    int GameObject::SpecialSegment() {
        if (events.empty()) {
            std::cout << "Нет доступных событий.\n";
            waitForEnter();
            return 0;
        }
        int idx = rand() % events.size();
        const Event& e = events[idx];
        system("cls");
        printSeparator('=', 80);
        std::cout << "СОБЫТИЕ\n";
        printSeparator('=', 80);
        std::cout << e.description << "\n\n";
        printSeparator('-', 80);
        applyEvent(e);
        printSeparator('=', 80);
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore(1000, '\n');
        std::cin.get();
        system("cls");
        return 0;
    }
    int GameObject::ShopSegment() {
        const int SHOP_CARDS_COUNT = 5;
        struct ShopCard {
            CardItem card;
            int price;
        };
        std::vector<ShopCard> shopCards;
        int playerLevel = player.getLevel();

        auto getRandomCardOfLevel = [&](int level) -> CardItem {
            std::vector<CardItem> candidates;
            for (const auto& c : cards) {
                if (c.level == level) candidates.push_back(c);
            }
            if (candidates.empty() && !cards.empty()) return cards[0];
            if (candidates.empty()) return CardItem();
            return candidates[rand() % candidates.size()];
            };

        for (int i = 0; i < SHOP_CARDS_COUNT; ++i) {
            int targetLevel = playerLevel;
            if (playerLevel < 5) {
                int r = rand() % 100;
                if (r < 20) targetLevel = std::max(1, playerLevel - 1);
                else if (r < 80) targetLevel = playerLevel;
                else if (r < 95) targetLevel = std::min(5, playerLevel + 1);
                else targetLevel = std::min(5, playerLevel + 2);
            }
            else {
                targetLevel = 5;
            }
            CardItem chosen = getRandomCardOfLevel(targetLevel);
            shopCards.push_back({ chosen, chosen.moneycost });
        }

        bool exitShop = false;
        while (!exitShop) {
            system("cls");
            printSeparator('=', 80);
            std::cout << "МАГАЗИН\n";
            printSeparator('=', 80);
            std::cout << COLOR_YELLOW << "Ваши фишки: " << player.getMoney() << COLOR_RESET << "\n\n";

            if (shopCards.empty()) {
                std::cout << "В магазине больше нет карт.\n";
                std::cout << "\n0 - Выйти из магазина\n";
                std::cout << "Ваш выбор: ";
                int choice;
                std::cin >> choice;
                if (choice == 0) exitShop = true;
                else std::cout << "Неверный ввод.\n";
                waitForEnter();
                continue;
            }

            std::cout << "Доступные карты:\n";
            for (size_t i = 0; i < shopCards.size(); ++i) {
                std::cout << i + 1 << ". " << shopCards[i].card.name
                    << " (ур." << shopCards[i].card.level
                    << ") - цена: " << shopCards[i].price << "\n";
            }
            std::cout << "\n0 - Выйти из магазина\n";
            std::cout << "9 - Осмотреть карту\n";
            std::cout << "Ваш выбор: ";
            int choice;
            std::cin >> choice;
            if (choice == 0) {
                exitShop = true;
                break;
            }
            else if (choice == 9) {
                std::cout << "Введите номер карты для осмотра (1.." << shopCards.size() << "): ";
                int inspectIdx;
                std::cin >> inspectIdx;
                if (inspectIdx >= 1 && inspectIdx <= (int)shopCards.size()) {
                    const CardItem& c = shopCards[inspectIdx - 1].card;
                    system("cls");
                    printSeparator('=', 80);
                    std::cout << "--- " << c.name << " ---\n";
                    std::cout << "Описание: " << c.description << "\n";
                    std::cout << "Уровень: " << c.level << "\n";
                    std::cout << "Стоимость ходов: " << c.cost << "\n";
                    std::cout << "Тип урона: ";
                    switch (c.dtype) {
                    case DamageType::physycs: std::cout << "Физический"; break;
                    case DamageType::mechanics: std::cout << "Механический"; break;
                    case DamageType::physycs_splash: std::cout << "Физический (сплеш)"; break;
                    case DamageType::heal: std::cout << "Лечение"; break;
                    case DamageType::unreal: std::cout << "Нереальный"; break;
                    default: std::cout << "Специальный"; break;
                    }
                    std::cout << "\nУрон: " << c.damage << "\n";
                    std::cout << "Цена: " << shopCards[inspectIdx - 1].price << "\n";
                    printSeparator('=', 80);
                    std::cout << "\nНажмите Enter для возврата...";
                    std::cin.ignore(1000, '\n');
                    std::cin.get();
                }
                else {
                    std::cout << "Неверный номер.\n";
                    waitForEnter();
                }
            }
            else if (choice >= 1 && choice <= (int)shopCards.size()) {
                ShopCard& selected = shopCards[choice - 1];
                if (player.getMoney() >= selected.price) {
                    player.addMoney(-selected.price);
                    player.getEnableCards().push_back(selected.card.id);
                    std::cout << COLOR_GREEN << "Вы купили карту \"" << selected.card.name << "\"!" << COLOR_RESET << "\n";
                    shopCards.erase(shopCards.begin() + (choice - 1));
                    waitForEnter();
                }
                else {
                    std::cout << COLOR_RED << "Недостаточно фишек!" << COLOR_RESET << "\n";
                    waitForEnter();
                }
            }
            else {
                std::cout << "Неверный ввод.\n";
                waitForEnter();
            }
        }
        return 0;
    }

    //graph functions
    GameObject::Graph GameObject::GenerateGraph() {
        Graph g;
        switch (currentFloor)
        {
        case 1:
            g = generateRoguelikeGraph(-1, FIRST_FLOOR_LAYERS, MIN_NODES_LAYERS, MAX_NODES_LAYERS,
                FIRST_FLOOR_BATTLES, FIRST_FLOOR_SHOPS, FIRST_FLOOR_SPECIALS,
                MAX_OUTGOING, MAX_EXTRA);
            break;
        case 2:
            g = generateRoguelikeGraph(-1, SECOND_FLOOR_LAYERS, MIN_NODES_LAYERS, MAX_NODES_LAYERS,
                SECOND_FLOOR_BATTLES, SECOND_FLOOR_SHOPS, SECOND_FLOOR_SPECIALS,
                MAX_OUTGOING, MAX_EXTRA);
            break;
        default:
            break;
        }
        g.print();
        return g;
    }

    std::vector<int> GameObject::generateLayer(Graph& graph, int numNodes,
        double weightBattle, double weightShop, double weightSpecial,
        int minBattleVal, int maxBattleVal,
        int minShopVal, int maxShopVal,
        int minSpecialVal, int maxSpecialVal)
    {
        std::vector<int> layer;
        double totalWeight = weightBattle + weightShop + weightSpecial;
        for (int i = 0; i < numNodes; ++i) {
            double r = randomDouble(0.0, totalWeight);
            RoomType type;
            int value = 0;
            if (r < weightBattle) {
                type = BATTLE;
                value = randomInt(minBattleVal, maxBattleVal);
            }
            else if (r < weightBattle + weightShop) {
                type = SHOP;
                value = randomInt(minShopVal, maxShopVal);
            }
            else {
                type = SPECIAL;
                value = randomInt(minSpecialVal, maxSpecialVal);
            }
            graph.addNode(type, value);
            layer.push_back((int)graph.nodes.size() - 1);
        }
        return layer;
    }

    bool GameObject::canConnect(const GameObject::Node& from, const GameObject::Node& to) {
        if (from.type == SHOP && to.type == SHOP) return false;
        if (from.type == SPECIAL && to.type == SPECIAL) return false;
        return true;
    }
    void GameObject::connectLayers(Graph& graph,
        const std::vector<int>& prevLayer,
        const std::vector<int>& nextLayer,
        int maxOutgoingPerNode,
        int maxExtraEdges)
    {
        if (prevLayer.empty() || nextLayer.empty()) return;
        std::vector<int> predCount(nextLayer.size(), 0);

        for (int from : prevLayer) {
            const Node& fromNode = graph.nodes[from];
            std::vector<int> candidates;
            for (int to : nextLayer)
                if (canConnect(fromNode, graph.nodes[to]))
                    candidates.push_back(to);
            if (candidates.empty()) candidates = nextLayer;
            int to = candidates[randomInt(0, (int)candidates.size() - 1)];
            graph.addEdge(from, to);
            auto it = std::find(nextLayer.begin(), nextLayer.end(), to);
            if (it != nextLayer.end()) predCount[it - nextLayer.begin()]++;
        }

        for (size_t i = 0; i < nextLayer.size(); ++i) {
            if (predCount[i] == 0) {
                int to = nextLayer[i];
                const Node& toNode = graph.nodes[to];
                std::vector<int> candidates;
                for (int from : prevLayer)
                    if (canConnect(graph.nodes[from], toNode))
                        candidates.push_back(from);
                if (candidates.empty()) candidates = prevLayer;
                int from = candidates[randomInt(0, (int)candidates.size() - 1)];
                graph.addEdge(from, to);
                predCount[i]++;
            }
        }

        for (int from : prevLayer) {
            int currentOut = (int)graph.nodes[from].next.size();
            int freeSlots = maxOutgoingPerNode - currentOut;
            int extra = std::min(maxExtraEdges, freeSlots);
            if (extra <= 0) continue;

            std::vector<int> candidates;
            for (int to : nextLayer)
                if (canConnect(graph.nodes[from], graph.nodes[to]))
                    candidates.push_back(to);
            if (candidates.empty()) candidates = nextLayer;

            std::set<int> targets;
            for (int i = 0; i < extra; ++i)
                targets.insert(candidates[randomInt(0, (int)candidates.size() - 1)]);
            for (int to : targets)
                graph.addEdge(from, to);
        }
    }
    GameObject::Graph GameObject::generateRoguelikeGraph(
        int seed,
        int NodePathLen,
        int minNodesPerLayer, int maxNodesPerLayer,
        double weightBattle, double weightShop, double weightSpecial,
        int maxOutgoingPerNode, int maxExtraEdges,
        int minBattleVal, int maxBattleVal,
        int minShopVal, int maxShopVal,
        int minSpecialVal, int maxSpecialVal)
    {

        Graph graph;
        graph.addNode(START, 0);
        std::vector<std::vector<int>> layers;
        layers.push_back({ 0 });

        for (int i = 0; i < NodePathLen; ++i) {
            int num = randomInt(minNodesPerLayer, maxNodesPerLayer);
            auto layer = generateLayer(graph, num,
                weightBattle, weightShop, weightSpecial,
                minBattleVal, maxBattleVal,
                minShopVal, maxShopVal,
                minSpecialVal, maxSpecialVal);
            layers.push_back(layer);
        }

        graph.addNode(BOSS, 0);
        layers.push_back({ (int)graph.nodes.size() - 1 });

        for (size_t i = 0; i < layers.size() - 1; ++i)
            connectLayers(graph, layers[i], layers[i + 1], maxOutgoingPerNode, maxExtraEdges);

        if (layers.size() >= 2 && !layers[1].empty()) {
            int start = 0;
            graph.nodes[start].next.clear();
            for (int to : layers[1])
                graph.addEdge(start, to);
        }

        std::vector<std::set<int>> incoming(graph.nodes.size());
        for (const auto& node : graph.nodes)
            for (int to : node.next)
                incoming[to].insert(node.id);

        for (size_t l = 1; l < layers.size(); ++l) {
            for (int nodeId : layers[l]) {
                if (incoming[nodeId].empty() && l > 0) {
                    const auto& prev = layers[l - 1];
                    if (!prev.empty()) {
                        int from = prev[randomInt(0, (int)prev.size() - 1)];
                        graph.addEdge(from, nodeId);
                        incoming[nodeId].insert(from);
                    }
                }
            }
        }
        return graph;
    }
    void GameObject::printGraphAscii(const Graph& graph, int startId) {
        const int NODE_WIDTH = 11;
        const int NODE_HEIGHT = 5;
        const int GAP_HORZ = NODE_WIDTH;
        const int GAP_VERT = NODE_HEIGHT * 1.5;
        const int LEFT_MARGIN = 2;

        // 1. Определяем уровни
        std::map<int, int> nodeLevel;
        std::queue<int> q;
        nodeLevel[startId] = 0;
        q.push(startId);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            int curLevel = nodeLevel[u];
            for (int v : graph.nodes[u].next) {
                if (nodeLevel.find(v) == nodeLevel.end() || nodeLevel[v] < curLevel + 1) {
                    nodeLevel[v] = curLevel + 1;
                    q.push(v);
                }
            }
        }

        // 2. Группируем узлы по уровням
        int maxLevel = 0;
        for (const auto& p : nodeLevel) maxLevel = std::max(maxLevel, p.second);
        std::vector<std::vector<int>> layers(maxLevel + 1);
        for (const auto& p : nodeLevel) layers[p.second].push_back(p.first);
        for (auto& layer : layers) std::sort(layer.begin(), layer.end());

        // 3. Для каждого слоя вычисляем его ширину и смещение для центрирования
        std::vector<int> layerWidth(layers.size());
        std::vector<int> layerOffset(layers.size());
        int maxWidth = 0;
        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            int numNodes = layers[lvl].size();
            int width = numNodes * NODE_WIDTH + (numNodes - 1) * GAP_HORZ;
            if (numNodes == 0) width = 0;
            layerWidth[lvl] = width;
            if (width > maxWidth) maxWidth = width;
        }
        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            layerOffset[lvl] = (maxWidth - layerWidth[lvl]) / 2;
        }

        // 4. Позиции узлов
        std::map<int, std::pair<int, int>> nodePos;
        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            for (size_t idx = 0; idx < layers[lvl].size(); ++idx) {
                nodePos[layers[lvl][idx]] = { lvl, idx };
            }
        }

        // 5. Размеры полотна
        int totalWidth = LEFT_MARGIN + maxWidth;
        int totalHeight = layers.size() * NODE_HEIGHT + (layers.size() - 1) * GAP_VERT;
        std::vector<std::string> canvas(totalHeight, std::string(totalWidth, ' '));

        // 6. Функция рисования узла без цвета
        auto drawNodeNoColor = [&](int level, int idx, int nodeId) {
            int y0 = level * (NODE_HEIGHT + GAP_VERT);
            int x0 = LEFT_MARGIN + layerOffset[level] + idx * (NODE_WIDTH + GAP_HORZ);
            const Node& node = graph.nodes[nodeId];

            for (int x = 0; x < NODE_WIDTH; ++x) {
                canvas[y0][x0 + x] = (x == 0 || x == NODE_WIDTH - 1) ? '+' : '-';
                canvas[y0 + NODE_HEIGHT - 1][x0 + x] = (x == 0 || x == NODE_WIDTH - 1) ? '+' : '-';
            }
            for (int y = 1; y < NODE_HEIGHT - 1; ++y) {
                canvas[y0 + y][x0] = '|';
                canvas[y0 + y][x0 + NODE_WIDTH - 1] = '|';
            }

            std::string idStr = std::to_string(node.id);
            int idStart = x0 + (NODE_WIDTH - (int)idStr.size()) / 2;
            for (size_t i = 0; i < idStr.size(); ++i) canvas[y0 + 1][idStart + i] = idStr[i];

            std::string typeStr;
            switch (node.type) {
            case START:   typeStr = "START"; break;
            case BATTLE:  typeStr = "BATTLE"; break;
            case SHOP:    typeStr = "SHOP"; break;
            case SPECIAL: typeStr = "SPECIAL"; break;
            case BOSS:    typeStr = "BOSS"; break;
            default:      typeStr = "??"; break;
            }
            int typeStart = x0 + (NODE_WIDTH - (int)typeStr.size()) / 2;
            for (size_t i = 0; i < typeStr.size(); ++i) canvas[y0 + 2][typeStart + i] = typeStr[i];

            std::string valStr = std::to_string(node.value);
            int valStart = x0 + (NODE_WIDTH - (int)valStr.size()) / 2;
            for (size_t i = 0; i < valStr.size(); ++i) canvas[y0 + 3][valStart + i] = valStr[i];
            };

        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            for (size_t idx = 0; idx < layers[lvl].size(); ++idx) {
                drawNodeNoColor(lvl, idx, layers[lvl][idx]);
            }
        }

        // 7. Рисуем рёбра
        for (size_t lvl = 0; lvl < layers.size() - 1; ++lvl) {
            for (int u : layers[lvl]) {
                int uLevel = nodePos[u].first;
                int uIdx = nodePos[u].second;
                int y0_u = uLevel * (NODE_HEIGHT + GAP_VERT);
                int x0_u = LEFT_MARGIN + layerOffset[uLevel] + uIdx * (NODE_WIDTH + GAP_HORZ);
                int x_center_u = x0_u + NODE_WIDTH / 2;
                int y_bottom_u = y0_u + NODE_HEIGHT - 1;

                for (int v : graph.nodes[u].next) {
                    if (nodePos.find(v) == nodePos.end()) continue;
                    int vLevel = nodePos[v].first;
                    if (vLevel != uLevel + 1) continue;
                    int vIdx = nodePos[v].second;
                    int y0_v = vLevel * (NODE_HEIGHT + GAP_VERT);
                    int x0_v = LEFT_MARGIN + layerOffset[vLevel] + vIdx * (NODE_WIDTH + GAP_HORZ);
                    int x_center_v = x0_v + NODE_WIDTH / 2;
                    int y_top_v = y0_v;

                    int x1 = x_center_u, y1 = y_bottom_u + 1;
                    int x2 = x_center_v, y2 = y_top_v - 1;
                    if (y1 > y2) continue;
                    int dx = x2 - x1, dy = y2 - y1;
                    int steps = std::max(std::abs(dx), std::abs(dy));
                    if (steps == 0) continue;
                    double xInc = (double)dx / steps, yInc = (double)dy / steps;
                    double x = x1, y = y1;
                    int prevX = x1, prevY = y1;
                    for (int i = 0; i <= steps; ++i) {
                        int ix = (int)std::round(x), iy = (int)std::round(y);
                        if (iy >= 0 && iy < totalHeight && ix >= 0 && ix < totalWidth) {
                            if (i == 0) canvas[iy][ix] = '|';
                            else {
                                int stepX = ix - prevX, stepY = iy - prevY;
                                if (stepX == 0 && stepY > 0) canvas[iy][ix] = '|';
                                else if (stepX > 0 && stepY == 0) canvas[iy][ix] = '-';
                                else if (stepX > 0 && stepY > 0) canvas[iy][ix] = '/';
                                else if (stepX < 0 && stepY > 0) canvas[iy][ix] = '\\';
                                else canvas[iy][ix] = '*';
                            }
                            prevX = ix; prevY = iy;
                        }
                        x += xInc; y += yInc;
                    }
                }
            }
        }

        // 8. Вывод с цветами
        struct Rect { int x0, y0, x1, y1; int nodeId; };
        std::vector<Rect> rects;
        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            for (size_t idx = 0; idx < layers[lvl].size(); ++idx) {
                int nodeId = layers[lvl][idx];
                int y0 = lvl * (NODE_HEIGHT + GAP_VERT);
                int x0 = LEFT_MARGIN + layerOffset[lvl] + idx * (NODE_WIDTH + GAP_HORZ);
                rects.push_back({ x0, y0, x0 + NODE_WIDTH - 1, y0 + NODE_HEIGHT - 1, nodeId });
            }
        }

        auto printLineWithColors = [&](int y, const std::string& line) {
            for (int x = 0; x < (int)line.size(); ++x) {
                char ch = line[x];
                int nodeId = -1;
                for (const auto& r : rects) {
                    if (y >= r.y0 && y <= r.y1 && x >= r.x0 && x <= r.x1) {
                        nodeId = r.nodeId;
                        break;
                    }
                }
                if (nodeId != -1) {
                    const Node& node = graph.nodes[nodeId];
                    const char* color = COLOR_RESET;
                    switch (node.type) {
                    case START:   color = COLOR_GREEN;   break;
                    case BATTLE:  color = COLOR_RED;     break;
                    case SHOP:    color = COLOR_BLUE;    break;
                    case SPECIAL: color = COLOR_YELLOW;  break;
                    case BOSS:    color = COLOR_MAGENTA; break;
                    }
                    std::cout << color << ch << COLOR_RESET;
                }
                else {
                    std::cout << ch;
                }
            }
            std::cout << '\n';
            };

        for (int y = 0; y < totalHeight; ++y) {
            printLineWithColors(y, canvas[y]);
        }
    }
    void GameObject::generateNshowdefaultgraph() {
        double battleWeight = 1.7;
        double shopWeight = 1.0;
        double specialWeight = 1.0;

        Graph g = generateRoguelikeGraph(-1, 5, 2, 4, battleWeight, shopWeight, specialWeight, 3, 2);
        g.print();
    }
}