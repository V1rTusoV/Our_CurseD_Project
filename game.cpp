#include "game.h"
#include <cstdlib>   
#include <ctime>     
#include <algorithm> 
#include <set>

namespace game {

    void GameObject::play() {
        initgame();
        bool flag = true;
        while (flag) {
            MapSegment();
            flag = false;
        }
    }

    void GameObject::initgame() {
        //srand(time(NULL));
        srand(1337);
        seed = (int) rand % 100000;
        currentFloor = 2;
    }

    void GameObject::MapSegment() {
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
    }

    void GameObject::BattleSegment() {
        
    }

    void GameObject::GenerateGraph() {
        
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