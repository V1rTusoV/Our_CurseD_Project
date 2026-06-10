#pragma once

#ifndef GAME_H
#define GAME_H

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




namespace game {

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

    class Base {
    public:
        int id;
        std::string codifier;
    };

    class CardItem : public Base {
    public:
        std::string name;
        std::string description;
        int level;
        int cost;
        DamageType dtype;
        int damage;

        CardItem(int Id = 0, std::string Codifier = " ", std::string Name = " ", std::string Description = " ", int Level = 0, int Cost = 0, DamageType Dtype = DamageType::specific, int Damage = 0)
        {
			id = Id;
			codifier = Codifier;
			name = Name;
			description = Description;
			level = Level;
			cost = Cost;
			dtype = Dtype;
			damage = Damage;
        }
    };

    class Enemy : public Base {
    public:
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

    public:
        Enemy(int Id = 0, std::string Codifietr = "", std::string Name = "",
            std::string Description = "", int Level = 1, int Initiative = 10, int Hp = 100, int Moves = 2, int Mechresist = 0, int Physresist = 0, int Splashresist = 0, int Maxhp = 10, int Maxmoves = 4) {
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
        }
    };

    class Player {
        int level;
        int initiative;
        int hp;
        int maxHp;               // добавлено
        int maxmoves;
        int moves;
        int money;
        std::vector<int> enableCards;
        std::vector<int> hand;
        int mechresist;
        int physresist;
        int splashresist;
    public:
        Player() : level(1), initiative(5), hp(10), maxHp(10), maxmoves(2), money(0),
            mechresist(2), physresist(1), splashresist(2) {
            enableCards = { 1,2,3,4,5,1,2,1,2,3};
            hand = {};
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
    };

    class GameObject {
    public:


        // Graph generation structures

        int randomInt(int min, int max) {
            return min + rand() % (max - min + 1);
        }

        double randomDouble(double min, double max) {
            return min + (rand() / (double)RAND_MAX) * (max - min);
        }

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

        // Settings
        float totalvolume;
        float musicvolume;
        float effectvolume;
        float soundvolume;

        // CurrentGame
        int seed;
        time_t runstart;
        Player player;
        int currentFloor;
        int currentNodeId;
        Graph map;
		std::vector <Enemy> enemys;
		std::vector <CardItem> cards;

        void printSeparator(char ch = '=', int width = 100);
        void printMainHeader();
        void printTopStats(const std::vector<game::Enemy>& battleEnemies, int playerHp, int playerMaxHp);
        void printHandAndMoves(const std::vector<int>& playerHand, int playerMovesRemaining, const std::vector<game::CardItem>& cards);
        void printConsequences(const std::string& message);
        void printInspectMenu(const std::vector<game::Enemy>& battleEnemies);
        void waitForEnter();
        void printInspectCards(const std::vector<int>& playerHand, const std::vector<game::CardItem>& cards);

        void initapp();
        void initgame();
        void play();
        int MapSegment();
        int BattleSegment();
        int ShopSegment();
        int SpecialSegment();

    };

}

#endif