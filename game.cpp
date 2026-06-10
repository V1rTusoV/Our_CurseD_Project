#include "game.h"
#include "FileWork.h"
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>
#include <queue>

namespace game {

    void GameObject::play() {
        initgame();
        bool flag = true;
        int gcode = 0;

        file::FileManager fm;
        
        std::vector<game::Enemy> enemies = {

			//id , codifier , name , description , level , initiative , hp , moves , mechresist , physresist , splashresist

            {1, "ENM", "Кот-Чести", "Занимает низшую иерархию в криминальном мире города К. Интересно, ты будешь таким же?", 1, 2, 5, 1, 0, 1, 0,5,1},
            {2, "ENM", "Кот-Солдат", "<Вы привыкли стрелять и нет места, куда податься? Рады приветствовать вас в семье!> из рекламной кампании семьи К.", 1, 1, 5, 1, 2, 2, 3,5,1},
			{3, "ENM", "Кот-бандит", "Улицы переполнены сбродом и это - их главный представитель.", 2, 1, 7, 1, 2, 3, 2,7,1},
			{4, "ENM", "Кот-убийца", "Наемный убийца, который может нанести вам серьезный урон, если вы не будете осторожны.", 2, 3, 10, 3, 3, 4, 2,10,3},
            {5, "ENM", "Хозяин улиц", "Представитель власти семьи на улицах города К.", 3, 10, 20, 4, 5, 7, 4,20,4},
            {6, "ENM", "Кот-Громила", "На удивление они занимаются тем, что громят.", 2, 5, 8, 3, 3, 3, 4,8,3},
            {7, "ENM", "Кот-стрелок", "Отчаянные ребята. Ни одной пули не устрашатся", 3, 6, 10, 2, 5, 3, 5,10,2},
            {8, "ENM", "Кот-подрывник", "БУМ!", 3, 6, 10, 2, 2, 2, 7,10,2},
            {9, "ENM", "Кот-дон", "Правая ежовая руковица главы семьи.", 4, 10, 25, 5, 6, 6, 4,25,5},
            {10, "ENM", "Великий и ужасный глава семьи - Мехин", "Пожалуйте все ваши фишки на стол, господа!", 5, 15, 35, 6, 10, 10, 10,35,6},
            {11, "NPC", "КоТ-Ерев - он же ваш крупье на все игры", "Непревзойденный крупье! Ослепительный ведущий! И никакого мухлежа", 100, 9999, 9999, 9999, 9999, 9999, 9999,9999,9999},
            {12, "NPC", "Пабло Котобар", "Продает самый лучший товар на рынке. Бери скорее, не пожалеешь!", 100, 100, 1000, 100, 9999, 9999, 9999,1000,100},
            {13, "NPC", "Виктор Кот", "Семье нужны пушки - Виктор дает пушки. К дальнейшему сотрудничеству мы не готовы.", 100, 100, 1000, 100, 9999, 9999, 9999,1000,100},
            {14, "ENM", "Кот-генерал", "Высокопоставленный член семьи К., который может вызвать подкрепление в виде других котов.", 5, 4, 25, 3, 5, 6, 4,25,3 },
        };
		enemies[0].enableCards = { 1,2 }; //Кот-Чести 
		enemies[1].enableCards = { 2,3,4,5,8 }; //Кот-Солдат 
		enemies[2].enableCards = { 1,2,3,4,17}; //Кот-бандит 
		enemies[3].enableCards = { 7,8 }; //Кот-убийца  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		enemies[4].enableCards = { 2,3,5,6,9,15,18}; //Хозяин улиц 
		enemies[5].enableCards = { 2,3,4,5,6,8 }; //Кот-Громила 
		enemies[6].enableCards = { 3,5,9,15 }; //Кот-стрелок 
		enemies[7].enableCards = { 1,2,3,4,7,16 }; //Кот-подрывник
		enemies[8].enableCards = { 3,5,6,7,8,9,12,14,15,18 }; //Кот-дон 
		enemies[9].enableCards = { 5,6,7,8,9,10,11,12,14,15,19,20,21 }; //Великий и ужасный глава семьи - Мехин 
		enemies[10].enableCards = { 10 }; //КоТ-Ерев 
		enemies[11].enableCards = { 8,9 }; //Пабло Котобар 
		enemies[12].enableCards = { 1}; //Виктор Кот 
		enemies[13].enableCards = { 1,2 }; //Кот-генерал !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        std::vector<game::CardItem> cards = {
            //CardItem(int Id, std::string Codifier, std::string Name, std::string Description, int Level, int Cost, DamageType Dtype, int Damage)
            //{
			//id = Id;
			//codifier = Codifier;
			//name = Name;
			//description = Description;
			//level = Level;
			//cost = Cost;
			//dtype = Dtype;
			//damage = Damage;
            //}
			{1,  "CRD", "Плевок шерстью", "мгъя!", 1, 1, game::DamageType::physycs, 1},
			{2,  "CRD", "удар лапкой", "Пушистое наслаждение. Вас погладили по-кошачьи.", 1, 1, game::DamageType::physycs, 1},
			{3,  "CRD", "удар когтями", "Цап-царапка!", 2, 1, game::DamageType::physycs, 2},
			{4,  "CRD", "Вкусный обед", "Блоунти - райское наслаждение.", 1, 1, game::DamageType::heal, 2},
			{5,  "CRD", "Кольт", "Что может быть лучше нестареющей классики?", 2, 4, game::DamageType::mechanics, 6},
			{6,  "CRD", "Стилеты", "Как там говорилось? Или вилкой в глаз, или...Не важно, запомни одно. Никогда не выбриай стилет!", 3, 3, game::DamageType::physycs, 4},
			{7,  "CRD", "Мышеловка с динамитной шашкой", "Да, Вы все верно прочитали.", 4, 7, game::DamageType::physycs_splash, 6},
			{8,  "CRD", "Пакет кошачьей мяты", "Целебное средство, о котором знает всякий зверь города К.", 1, 3, game::DamageType::heal, 5},
			{9,  "CRD", "Мешок кошачьей мяты", "Товар, благодаря которому известный Котальпоне обрел свою известность.", 3, 5, game::DamageType::heal, 10},
            {10, "CRD", "Гримуар по Це Пе Пе","Лишь два кота в городе К. владеют знаниями, записанные в этом гримуаре. И признаюсь честно, не желал бы я пересечь им дорогу.",9999,9999,game::DamageType::unreal,9999},

            {11, "CRD", "Томми-ган", "Старый добрый Томм", 5, 10, game::DamageType::mechanics, 12},
            {12, "CRD", "Лупара", "В нашем деле скорость решающий фактор.", 4, 8, game::DamageType::mechanics, 10},
            {13, "CRD", "Девять жизней", "Хм? Так сказки о девяти кошачьих жизнях - не ложь? Что же, надеюсь, что хотя бы эту жизнь ты будешь ценить.", 5, 0, game::DamageType::heal, 0}, //!!!!!!!!!!!!
            {14, "CRD", "Винчестер", "Надежнее оружия вам не найти.", 4, 5, game::DamageType::mechanics, 5},
            {15, "CRD", "Маузер", "Он из Германия прибыть. Целебных пулей привозить.", 3, 3, game::DamageType::mechanics, 4},
            {16, "CRD", "Наган 1895", "Хм, кажется там остался всего один патрон.", 5, 0, game::DamageType::mechanics, 0}, //!!!!!!!!!!!!!!!!!!!
            {17, "CRD", "Деревянная дубинка", "За неимением лучшего.", 2, 3, game::DamageType::physycs, 2},
            {18, "CRD", "Стальная дубинка", "За неимением наилучшего.", 3, 4, game::DamageType::physycs, 4},
            {19, "CRD", "Привет из будущего", "Это миниган?", 5, 10, game::DamageType::mechanics, 20},
            {20, "CRD", "Накодил","SDL_Texture*...Так, что там дальше...",5,10,game::DamageType::summon,2}, //!!!!!!!!!!!!!!!!

            {21, "CRD", "Кавардак", "-то здесь не Хмм, что так, определенно не вот что", 5, 10, game::DamageType::specific, 0 }, //!!!!!!!!!!!!!!!!!

        };





        fm.writeEnemiesToFile("enemies.json", enemies);
        auto loaded = fm.readEnemiesFromFile("enemies.json");
        std::cout << "Загружено " << loaded.size() << " врагов\n";

		fm.writeCardsToFile("cards.json", cards);
		auto loaded2 = fm.readCardsFromFile("cards.json");
		std::cout << "Загружено " << loaded2.size() << " карт\n";

        while (flag) {
            gcode = MapSegment();
            switch (gcode) {
            case 1:
                if (BattleSegment() == 0) flag = 0;   // если игрок умер – выход
                std::cout << "BATTLE!" << std::endl;
                break;
            case 2:
                // ShopSegment(); // пока заглушка
                std::cout << "SHOP!" << std::endl;
                break;
            case 3:
                // SpecialSegment(); // пока заглушка
                std::cout << "SPECIAL!" << std::endl;
                break;
            case 4:
                std::cout << "current floor:" << currentFloor << std::endl;
                std::cout << "BOSS BATTLE!" << std::endl;
                std::cout << "next floor!" << std::endl;
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

	void GameObject::initapp() {
		totalvolume = 1.0f;
		musicvolume = 1.0f;
		effectvolume = 1.0f;
		soundvolume = 1.0f;
	}

    void GameObject::initgame() {
        srand(1);
        seed = abs(1 + (int)rand % 10000);
        currentFloor = 1;
        runstart = time(nullptr);
        currentNodeId = 0;
        map = GenerateGraph();

        // Загрузка данных из JSON
        file::FileManager fm;
        enemys = fm.readEnemiesFromFile("enemies.json");
        cards = fm.readCardsFromFile("cards.json");
        std::cout << "Loaded " << enemys.size() << " enemies, " << cards.size() << " cards\n";
    }

    int GameObject::MapSegment() {
        int returnval = 0;

        std::cout << enemys[0].maxhp << std::endl;
        std::cout << enemys[1].maxhp << std::endl;
        std::cout << enemys[2].maxhp << std::endl;
        std::cout << enemys[3].maxhp << std::endl;
        std::cout << enemys[4].maxhp << std::endl;
        std::cout << enemys[5].maxhp << std::endl;


        // Глобальные настройки отображения
        
        // 0 - Завершение игры
        // 1 - Переход в боевую сцену
		// 2 - Переход в сцену магазина
        // 3 - Переход в сцену SPECIAL
        // 4 - Переход на след. этаж

        printGraphAscii(map, currentNodeId);

        std::cout << "Enter yr next node to travel: ";
        std::cin >> currentNodeId;

        switch (map.nodes[currentNodeId].type)
        {
        case BATTLE:
            return 1;
            break;
        case SHOP:
            return 2;
            break;
		case SPECIAL:
            return 3;
            break;
        case BOSS:
            currentFloor++;
            return 4;
            break;
        default:
            return 404;
            break;
        }
    }

    const int SEPARATOR_WIDTH = 80;

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
        // Игрок (синий)
        std::cout << COLOR_BLUE << "Игрок:" << COLOR_RESET << "\n";
        std::cout << COLOR_BLUE << "  Хп: " << playerHp << "/" << playerMaxHp << COLOR_RESET << "\n\n";

        // Враги (красные)
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
    
    int GameObject::BattleSegment() {
        // ------------------------------------------------------------------
        // 1. Выбор противников (копии из enemys)
        // ------------------------------------------------------------------
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

        // ------------------------------------------------------------------
        // 2. Локальное состояние игрока
        // ------------------------------------------------------------------
        int playerHp = player.getHp();
        int playerMaxHp = player.getMaxHp();
        std::vector<int> playerEnableCards = player.getEnableCards();
        std::vector<int> playerHand;  // рука сохраняется между ходами
        int playerMovesRemaining = 0;

        // ------------------------------------------------------------------
        // 3. Вспомогательные лямбды
        // ------------------------------------------------------------------
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

        // ------------------------------------------------------------------
        // 4. Основной цикл боя
        // ------------------------------------------------------------------
        bool playerAlive = true;
        bool enemiesAlive = true;

        while (playerAlive && enemiesAlive) {
            // Сбор участников по инициативе
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
                    // ---------- ХОД ИГРОКА ----------
                    playerMovesRemaining = player.getMaxMoves();
                    // Добор до 2 карт, но не более 6 в руке
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
                    // НЕ возвращаем оставшиеся карты! Они остаются в руке на следующий ход.
                    // Только разыгранные карты уже возвращены через returnCardToEnable.
                }
                else {
                    // ---------- ХОД ВРАГА ----------
                    Enemy& enemy = battleEnemies[turn.enemyIndex];
                    if (enemy.hp <= 0) continue;

                    int enemyMoves = enemy.moves;
                    drawCards(enemy.hand, enemy.enableCards, 2, 6);
                    bool played = false;
                    std::string consequence;

                    // Лечение при низком HP
                    if (enemy.hp < enemy.maxhp / 2) {
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
                        // Атака
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

        // ------------------------------------------------------------------
        // 5. Завершение боя (без рамок ||)
        // ------------------------------------------------------------------
        if (!playerAlive) {
            printSeparator('=', 80);
            std::cout << "ВЫ ПОВЕРЖЕНЫ...\n";
            printSeparator('=', 80);
            waitForEnter();
            return 0;
        }
        else {
            player.setHp(playerHp);
            player.getEnableCards() = playerEnableCards;
            printSeparator('=', 80);
            std::cout << "ПОБЕДА!\n";
            printSeparator('=', 80);
            waitForEnter();
            return 1;
        }
    }
    
    int GameObject::ShopSegment() {


        return 0;
    }
    int GameObject::SpecialSegment() {


        return 0;
    }

    GameObject::Graph GameObject::GenerateGraph() {
        Graph g;
        switch (currentFloor)
        {
        case 1:
            g = generateRoguelikeGraph(seed, FIRST_FLOOR_LAYERS, MIN_NODES_LAYERS, MAX_NODES_LAYERS, FIRST_FLOOR_BATTLES, FIRST_FLOOR_SHOPS, FIRST_FLOOR_SPECIALS, MAX_OUTGOING, MAX_EXTRA);
            break;
        case 2:
            g = generateRoguelikeGraph(seed, SECOND_FLOOR_LAYERS, MIN_NODES_LAYERS, MAX_NODES_LAYERS, SECOND_FLOOR_BATTLES, SECOND_FLOOR_SHOPS, SECOND_FLOOR_SPECIALS, MAX_OUTGOING, MAX_EXTRA);
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
        if (seed >= 0) srand(seed);
        else srand((unsigned)time(NULL));

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
        const int NODE_WIDTH = 11;          // ширина блока (символов)
        const int NODE_HEIGHT = 5;          // высота блока
        const int GAP_HORZ = NODE_WIDTH;    // расстояние между блоками = целый блок
        const int GAP_VERT = NODE_HEIGHT * 1.5; // вертикальный отступ (7-8 символов)
        const int LEFT_MARGIN = 2;          // небольшой отступ слева для красоты

        // 1. Определяем уровни (BFS)
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
        for (auto& layer : layers) std::sort(layer.begin(), layer.end()); // сортируем по id

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

        // 4. Позиции узлов (уровень, индекс в слое)
        std::map<int, std::pair<int, int>> nodePos; // id -> (level, index)
        for (size_t lvl = 0; lvl < layers.size(); ++lvl) {
            for (size_t idx = 0; idx < layers[lvl].size(); ++idx) {
                nodePos[layers[lvl][idx]] = { lvl, idx };
            }
        }

        // 5. Размеры полотна
        int totalWidth = LEFT_MARGIN + maxWidth;
        int totalHeight = layers.size() * NODE_HEIGHT + (layers.size() - 1) * GAP_VERT;
        std::vector<std::string> canvas(totalHeight, std::string(totalWidth, ' '));

        // 6. Функция рисования узла без цвета (только символы)
        auto drawNodeNoColor = [&](int level, int idx, int nodeId) {
            int y0 = level * (NODE_HEIGHT + GAP_VERT);
            int x0 = LEFT_MARGIN + layerOffset[level] + idx * (NODE_WIDTH + GAP_HORZ);
            const Node& node = graph.nodes[nodeId];

            // Рамка
            for (int x = 0; x < NODE_WIDTH; ++x) {
                canvas[y0][x0 + x] = (x == 0 || x == NODE_WIDTH - 1) ? '+' : '-';
                canvas[y0 + NODE_HEIGHT - 1][x0 + x] = (x == 0 || x == NODE_WIDTH - 1) ? '+' : '-';
            }
            for (int y = 1; y < NODE_HEIGHT - 1; ++y) {
                canvas[y0 + y][x0] = '|';
                canvas[y0 + y][x0 + NODE_WIDTH - 1] = '|';
            }

            // ID
            std::string idStr = std::to_string(node.id);
            int idStart = x0 + (NODE_WIDTH - (int)idStr.size()) / 2;
            for (size_t i = 0; i < idStr.size(); ++i) canvas[y0 + 1][idStart + i] = idStr[i];

            // Тип
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

            // Значение
            std::string valStr = std::to_string(node.value);
            int valStart = x0 + (NODE_WIDTH - (int)valStr.size()) / 2;
            for (size_t i = 0; i < valStr.size(); ++i) canvas[y0 + 3][valStart + i] = valStr[i];
            };

        // Рисуем все узлы
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
        // Собираем все прямоугольники узлов для быстрой проверки принадлежности
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

        Graph g = generateRoguelikeGraph(
            1335,
            5,          
            2, 4,       
            battleWeight, shopWeight, specialWeight,
            3, 2        
        );
        g.print();
    }
}