#pragma once

#ifndef GAME_H
#define GAME_H

//Include block
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
#include <iomanip>
#include <queue>
#include <cstdlib>
#include <cctype>
#include <limits>

//STATIC GAME DEFINE BLOCK
#define MAX_FLOOR 2

//BATTLE DEFINE BLOCK
#define MAX_ENEMY_IN_BATTLE 3

//GRAPH DEFINE BLOCK
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"

#define MIN_NODES_LAYERS 2
#define MAX_NODES_LAYERS 3
#define MAX_OUTGOING 3
#define MAX_EXTRA 2

#define FIRST_FLOOR_LAYERS 8

#define FIRST_FLOOR_BATTLES 1.7
#define FIRST_FLOOR_SHOPS 1
#define FIRST_FLOOR_SPECIALS 1

#define SECOND_FLOOR_LAYERS 12

#define SECOND_FLOOR_BATTLES 2.2
#define SECOND_FLOOR_SHOPS 1.2
#define SECOND_FLOOR_SPECIALS 1.2


//game namespace
namespace game {

    //Enum block
    enum DamageType {
        physycs,
        heal,
        mechanics,
        unreal,
        physycs_splash,
        summon,
        specific
    };
    enum RoomType {
        START,
        BATTLE,
        SHOP,
        SPECIAL,
        BOSS
    };
    enum EventType {
        //codifier ::    EVT|(Сила/Карта)
        //Пример: (объект класса Event)
        // id:1
        // codifier:EVT|3
        // description: Проходя мимо вашего стола, член семьи К. уронил одну из своих кард
        // type: AddItem
        // Данный евент будет давать игроку в enablecards карту с id = 3

        AddItem,        //Добавить карту                                                    Кодификатор: EVT|(карта которую добавить)
        RemoveItem,     //Удалить случайную карту с enablecards игрока                      Кодификатор: EVT
        AddResist,      //Добавить к одному из трех сопротивлений значение                  Кодификатор: EVT|(к какому сопротивлению добавить)|(добавляемое значение)
        RemoveResist,   //Убрать из одного из трех сопротивлений значение                   Кодификатор: EVT|(из какого сопротивления убрать)|(убираемое значение)
        AddMoney,       //Добавить игроку фишки                                             Кодификатор: EVT|(сколько фишек добавить)
        RemoveMoney,    //Забрать у игрока фишки                                            Кодификатор: EVT|(сколько фишек убрать)
        AddMoves,       //Навсегда увеличить количество ходов у игрока                      Кодификатор: EVT|(сколько ходов добавить)
        RemoveMoves,    //Навсегда уменьшить количество ходов у игрока                      Кодификатор: EVT|(сколько ходов убрать)
        AddMaxHp,       //Увеличить максимальное здоровье игрока                            Кодификатор: EVT|(сколько добавить)
        RemoveMaxHp,    //Уменьшить максимальное здоровье игрока                            Кодификатор: EVT|(сколько убрать)
        AddHp,          //Полечить игрока на значение                                       Кодификатор: EVT|(сколько востановить)
        RemoveHp        //Покалечить игрока(если хп опускается до 0 и ниже, оставить 1)     Кодификатор: EVT|(сколько убрать)
    };

    //Class block
    class Base {
    public:
        int id;
        std::string codifier;
    };
    class Event : public Base {
    public:

        std::string description;
        EventType type;

        int value1;   // первый параметр (например, id карты, количество денег, величина изменения)
        int value2;   // второй параметр (например, тип сопротивления: 0-мех,1-физ,2-сплеш)

        Event(int Id = 0, std::string Codifier = "", std::string Description = "",
            EventType Type = EventType::AddItem, int Val1 = 0, int Val2 = 0)
            : description(Description), type(Type), value1(Val1), value2(Val2) {
            id = Id;
            codifier = Codifier;
        }

        void SetId(int Id)
        {
            id = Id;
        }
        void SetCodifier(std::string Codifier)
        {
            codifier = Codifier;
        }
    };
    class CardItem : public Base {
    public:
        std::string img;
        std::string name;
        std::string description;
        int level;
        int cost;
        DamageType dtype;
        int damage;
        int moneycost;

        CardItem(int Id = 0, std::string Codifier = " ", std::string Name = " ", std::string Description = " ", int Level = 0,
            int Cost = 0, DamageType Dtype = DamageType::specific, int Damage = 0, int Moneycost = 10, std::string Img = "Temp")
        {
            id = Id;
            codifier = Codifier;
            name = Name;
            description = Description;
            level = Level;
            cost = Cost;
            dtype = Dtype;
            damage = Damage;
            moneycost = Moneycost;
            img = Img;
        }
    };
    class Enemy : public Base {
    public:
        std::string img;
        std::string name;
        std::string description;
        int level;
        int initiative;
        int hp;
        int maxhp;
        int moves;
        int maxmoves;
        std::vector<int> enableCards;
        std::vector<int> hand;
        int mechresist;
        int physresist;
        int splashresist;
        int gainexp;
        int gainmoney;

    public:
        Enemy(int Id = 0, std::string Codifietr = "", std::string Name = "",
            std::string Description = "", int Level = 1, int Initiative = 10, int Hp = 100, int Moves = 2, int Mechresist = 0, int Physresist = 0, int Splashresist = 0,
            int Maxhp = 10, int Maxmoves = 4, int Gainexp = 10, int Gainmoney = 10, std::string Img = "temp") {
            id = Id;
            codifier = Codifietr;
            name = Name;
            description = Description;
            level = Level;
            initiative = Initiative;
            hp = Hp;
            moves = Moves;
            mechresist = Mechresist;
            physresist = Physresist;
            splashresist = Splashresist;
            maxhp = Maxhp;
            maxmoves = Maxmoves;
            gainexp = Gainexp;
            gainmoney = Gainmoney;
            img = Img;
        }
    };
    class Player {
        int level;
        int initiative;
        int hp;
        int maxHp;
        int maxmoves;
        int moves;
        int money;
        int exp;
        std::vector<int> enableCards;
        std::vector<int> hand;
        int mechresist;
        int physresist;
        int splashresist;
    public:
        Player() : level(1), initiative(5), hp(15), maxHp(15), maxmoves(3), money(0),
            mechresist(2), physresist(1), splashresist(2) {
            enableCards = { 0,0,1,2,3,4,8,1,2,3,4,17 };
            hand = {};
        }

        void CheckLevel() {
            bool leveled = false;
            do {
                leveled = false;
                switch (level) {
                case 1: if (exp >= 50) { level++; exp -= 50; leveled = true; } break;
                case 2: if (exp >= 70) { level++; exp -= 70; leveled = true; } break;
                case 3: if (exp >= 100) { level++; exp -= 100; leveled = true; } break;
                case 4: if (exp >= 140) { level++; exp -= 140; leveled = true; } break;
                case 5: break;
                default: break;
                }
                if (leveled) {
                    maxHp += 5;
                    hp = maxHp;
                    if (level % 2 == 0) maxmoves++;
                }
            } while (leveled);
        }

        void addExp(int amount) { exp += amount; CheckLevel(); }

        int getExp() const { return exp; }
        int getExpForNextLevel() const {
            switch (level) {
            case 1: return 50;
            case 2: return 70;
            case 3: return 100;
            case 4: return 140;
            default: return 0;
            }
        }

        // Геттеры и сеттеры
        int getLevel() const { return level; }
        int getInitiative() const { return initiative; }
        int getHp() const { return hp; }
        int getMaxHp() const { return maxHp; }
        void setHp(int newHp) { hp = newHp; if (hp > maxHp) hp = maxHp; if (hp < 0) hp = 0; }
        int getMoves() const { return moves; }
        int getMaxMoves() const { return maxmoves; }
        int getMoney() const { return money; }
        void addMoney(int amount) { money += amount; }
        std::vector<int>& getEnableCards() { return enableCards; }
        const std::vector<int>& getEnableCards() const { return enableCards; }
        std::vector<int>& getHand() { return hand; }
        const std::vector<int>& getHand() const { return hand; }
        int getMechresist() const { return mechresist; }
        int getPhysresist() const { return physresist; }
        int getSplashresist() const { return splashresist; }
        void addMechresist(int amount) { mechresist += amount; if (mechresist < 0) mechresist = 0; }
        void addPhysresist(int amount) { physresist += amount; if (physresist < 0) physresist = 0; }
        void addSplashresist(int amount) { splashresist += amount; if (splashresist < 0) splashresist = 0; }
        void addMaxMoves(int amount) { maxmoves += amount; if (maxmoves < 1) maxmoves = 1; }
        void addMaxHp(int amount) {
            maxHp += amount;
            if (maxHp < 1) maxHp = 1;
            if (hp > maxHp) hp = maxHp;
        }
    };

    //GameObject block
    class GameObject {
    public:

        //Random block
        int randomInt(int min, int max) {
            return min + rand() % (max - min + 1);
        }
        double randomDouble(double min, double max) {
            return min + (rand() / (double)RAND_MAX) * (max - min);
        }

        //Graph
        struct Node {
            int id;
            RoomType type;
            int value;
            std::set<int> next;
        };

        class Graph {
        public:
            std::vector<Node> nodes;

            void addNode(RoomType type, int value) {
                nodes.push_back({ (int)nodes.size(), type, value, {} });
            }

            void addEdge(int from, int to) {
                nodes[from].next.insert(to);
            }

            void print() const {
                std::cout << "\n===== Graph Nodes =====\n";
                for (const auto& node : nodes) {
                    std::string typeStr;
                    switch (node.type) {
                    case RoomType::START:   typeStr = "START"; break;
                    case RoomType::BATTLE:  typeStr = "BATTLE"; break;
                    case RoomType::SHOP:    typeStr = "SHOP"; break;
                    case RoomType::SPECIAL: typeStr = "SPECIAL"; break;
                    case RoomType::BOSS:    typeStr = "BOSS"; break;
                    }
                    std::cout << "Node " << node.id << " (" << typeStr << ", value=" << node.value << ") -> ";
                    std::vector<int> nextVec(node.next.begin(), node.next.end());
                    for (size_t i = 0; i < nextVec.size(); ++i) {
                        std::cout << nextVec[i];
                        if (i + 1 < nextVec.size()) std::cout << ", ";
                    }
                    std::cout << "\n";
                }
                std::cout << "======================\n";
            }
        };

        Graph GenerateGraph();
        Graph generateRoguelikeGraph(
            int seed = -1,
            int NodePathLen = 5,
            int minNodesPerLayer = 2, int maxNodesPerLayer = 4,
            double weightBattle = 1, double weightShop = 1, double weightSpecial = 1,
            int maxOutgoingPerNode = 3,
            int maxExtraEdges = 2,
            int minBattleVal = -15, int maxBattleVal = -5,
            int minShopVal = 10, int maxShopVal = 20,
            int minSpecialVal = 5, int maxSpecialVal = 15
        );
        void generateNshowdefaultgraph();
        void printGraphAscii(const GameObject::Graph& graph, int startId);
        std::vector<int> generateLayer(Graph& graph, int numNodes,
            double weightBattle, double weightShop, double weightSpecial,
            int minBattleVal, int maxBattleVal,
            int minShopVal, int maxShopVal,
            int minSpecialVal, int maxSpecialVal);

        bool canConnect(const Node& from, const Node& to);

        void connectLayers(Graph& graph,
            const std::vector<int>& prevLayer,
            const std::vector<int>& nextLayer,
            int maxOutgoingPerNode,
            int maxExtraEdges);

        //Init Block
        void initapp();
        void initgame();

        //Show Block
        void printSeparator(char ch = '=', int width = 100);
        void printMainHeader();
        void printTopStats(const std::vector<game::Enemy>& battleEnemies, int playerHp, int playerMaxHp);
        void printHandAndMoves(const std::vector<int>& playerHand, int playerMovesRemaining, const std::vector<game::CardItem>& cards);
        void printConsequences(const std::string& message);
        void printInspectMenu(const std::vector<game::Enemy>& battleEnemies);
        void waitForEnter();
        void printInspectCards(const std::vector<int>& playerHand, const std::vector<game::CardItem>& cards);
        void ShowPlayerStats();

        //Game Block

        void applyEvent(const Event& e);
        void play();
        int MapSegment();
        int BattleSegment(bool isBoss = false);
        int ShopSegment();
        int SpecialSegment();

        //Settings
        float totalvolume;
        float musicvolume;
        float effectvolume;
        float soundvolume;

        //Game vars
        const int SEPARATOR_WIDTH = 80;
        int seed;
        time_t runstart;
        Player player;
        int currentFloor;
        int currentNodeId;
        Graph map;
        std::vector <Enemy> enemys;
        std::vector <CardItem> cards;
        std::vector<Event> events;
        std::vector<Enemy> commonEnemies;
        std::vector<Enemy> bossEnemies;
        std::vector<Enemy> availableBosses;

    };

}


#endif //GAME_H