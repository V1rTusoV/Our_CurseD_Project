#include "game.h"
#include "FileWork.h"
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>

namespace game {

    void GameObject::play() {
        initgame();
        bool flag = true;
        int gcode = 0;

        file::FileManager fm;

        std::vector<game::Enemy> enemies = {

			//id , codifier , name , description , level , initiative , hp , moves , mechresist , physresist , splashresist

            {1, "ENM", "Кот-Чести", "Занимает низшую иерархию в криминальном мире города К. Интересно, ты будешь таким же?", 1, 2, 5, 1, 0, 1, 0},
            {2, "ENM", "Кот-Солдат", "<Вы привыкли стрелять и нет места, куда податься? Рады приветствовать вас в семье!> из рекламной кампании семьи К.", 1, 1, 5, 1, 2, 2, 3},
			{3, "ENM", "Кот-бандит", "Улицы переполнены сбродом и это - их главный представитель.", 2, 1, 7, 1, 2, 3, 2},
			{4, "ENM", "Кот-убийца", "Наемный убийца, который может нанести вам серьезный урон, если вы не будете осторожны.", 2, 3, 10, 3, 3, 4, 2},
            {5, "ENM", "Хозяин улиц", "Представитель власти семьи на улицах города К.", 3, 10, 20, 4, 5, 7, 4},
            {6, "ENM", "Кот-Громила", "На удивление они занимаются тем, что громят.", 2, 5, 8, 3, 3, 3, 4},
            {7, "ENM", "Кот-стрелок", "Отчаянные ребята. Ни одной пули не устрашатся", 3, 6, 10, 2, 5, 3, 5},
            {8, "ENM", "Кот-подрывник", "БУМ!", 3, 6, 10, 2, 2, 2, 7},
            {9, "ENM", "Кот-дон", "Правая ежовая руковица главы семьи.", 4, 10, 25, 5, 6, 6, 4},
            {10, "ENM", "Великий и ужасный глава семьи - Мехин", "Пожалуйте все ваши фишки на стол, господа!", 5, 15, 35, 6, 10, 10, 10},
            {11, "NPC", "КоТ-Ерев - он же ваш крупье на все игры", "Непревзойденный крупье! Ослепительный ведущий! И никакого мухлежа", 100, 9999, 9999, 9999, 9999, 9999, 9999},
            {12, "NPC", "Пабло Котобар", "Продает самый лучший товар на рынке. Бери скорее, не пожалеешь!", 100, 100, 1000, 100, 9999, 9999, 9999},
            {13, "NPC", "Виктор Кот", "Семье нужны пушки - Виктор дает пушки. К дальнейшему сотрудничеству мы не готовы.", 100, 100, 1000, 100, 9999, 9999, 9999},
            {14, "ENM", "Кот-генерал", "Высокопоставленный член семьи К., который может вызвать подкрепление в виде других котов.", 5, 4, 25, 3, 5, 6, 4 },
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
		enemies[13].enableCards = { 27,28 }; //Кот-генерал !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        std::vector<game::CardItem> cards = {
            /*CardItem(int Id, std::string Codifier, std::string Name, std::string Description, int Level, int Cost, DamageType Dtype, int Damage)
            {
			id = Id;
			codifier = Codifier;
			name = Name;
			description = Description;
			level = Level;
			cost = Cost;
			dtype = Dtype;
			damage = Damage;
            }*/
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
            switch (gcode)
            {
            case 0:
                std::cout << "ZERO!" << std::endl;
                flag = 0;
                break;
            case 1:
                std::cout << "BATTLE!" << std::endl;
                break;
            case 2:
                std::cout << "SHOP!" << std::endl;
                break;
            case 3:
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
        }
    }

	void GameObject::initapp() {
		totalvolume = 1.0f;
		musicvolume = 1.0f;
		effectvolume = 1.0f;
		soundvolume = 1.0f;
	}

    void GameObject::initgame() {
        //srand(time(NULL));
        srand(1);

        seed = abs(1 + (int)rand % 10000);
        currentFloor = 1;
        runstart = time(nullptr);
        currentNodeId = 0;
        map = GenerateGraph();

        std::cout << seed << std::endl;
    }

    int GameObject::MapSegment() {
        int returnval = 0;

        // 0 - Завершение игры
        // 1 - Переход в боевую сцену
		// 2 - Переход в сцену магазина
        // 3 - Переход в сцену SPECIAL
        // 4 - Переход на след. этаж

		for (int n : map.nodes[currentNodeId].next) {
			std::cout << "Next node: " << n << " Type: " << map.nodes[n].type << " Value: " << map.nodes[n].value << "\n";
		}

        std::cout << "Enter yr next node to travel: ";
		std::cin >> currentNodeId;
        std::cout << "-------------------------------\n";

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
            std::cout << "BOOOOOOOOOOOOOS" << std::endl;
            if (currentFloor == MAX_FLOOR)
            {
                std::cout << "returning ZERO" << std::endl;
                return 0;
            }
            std::cout << "returning NOT ZERO" << std::endl;
            currentFloor++;
            return 4;
            break;
        default:
            return 404;
            break;
        }
    }

    int GameObject::BattleSegment() {
        

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

    void GameObject::InitializeEnemys() {
        
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