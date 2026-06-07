#include "game.h"
#include <cstdlib>   // для rand(), srand()
#include <ctime>     // для time()
#include <algorithm> // для std::find, std::min
#include <set>

namespace game {

    // ----------------------------------------------------------------------
    // Реализация методов GameObject (объявленных в game.h)
    // ----------------------------------------------------------------------

    void GameObject::play() {
        // Заглушка / временная реализация
        bool flag = true;
        while (flag) {
            // TODO: игровой цикл
        }
    }

    void GameObject::MapSegment() {
        // Заглушка
    }

    void GameObject::BattleSegment() {
        // Заглушка
    }

    void GameObject::GenerateGraph() {
        // Заглушка
    }

    void GameObject::InitializeEnemys() {
        // Заглушка
    }

    // ----------------------------------------------------------------------
    // Генерация одного слоя (узлов)
    // ----------------------------------------------------------------------
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

    // ----------------------------------------------------------------------
    // Проверка возможности соединения (запрет SHOP->SHOP и SPECIAL->SPECIAL)
    // ----------------------------------------------------------------------
    bool GameObject::canConnect(const GameObject::Node& from, const GameObject::Node& to) {
        if (from.type == SHOP && to.type == SHOP) return false;
        if (from.type == SPECIAL && to.type == SPECIAL) return false;
        return true;
    }

    // ----------------------------------------------------------------------
    // Соединение двух слоёв
    // ----------------------------------------------------------------------
    void GameObject::connectLayers(Graph& graph,
        const std::vector<int>& prevLayer,
        const std::vector<int>& nextLayer,
        int maxOutgoingPerNode,
        int maxExtraEdges)
    {
        if (prevLayer.empty() || nextLayer.empty()) return;
        std::vector<int> predCount(nextLayer.size(), 0);

        // Обязательные рёбра от каждого узла prevLayer
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

        // Гарантия предшественника для каждого узла nextLayer
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

        // Дополнительные случайные рёбра
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

    // ----------------------------------------------------------------------
    // Основная функция генерации графа
    // ----------------------------------------------------------------------
    GameObject::Graph GameObject::generateRoguelikeGraph(
        int NodePathLen,
        int minNodesPerLayer, int maxNodesPerLayer,
        double weightBattle, double weightShop, double weightSpecial,
        int maxOutgoingPerNode,
        int maxExtraEdges,
        int minBattleVal, int maxBattleVal,
        int minShopVal, int maxShopVal,
        int minSpecialVal, int maxSpecialVal,
        int seed)
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

        // Старт соединяется со всеми узлами первого слоя
        if (layers.size() >= 2 && !layers[1].empty()) {
            int start = 0;
            graph.nodes[start].next.clear();
            for (int to : layers[1])
                graph.addEdge(start, to);
        }

        // Исправление изолированных узлов (без входящих рёбер)
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

    // ----------------------------------------------------------------------
    // Демонстрация генерации графа по умолчанию
    // ----------------------------------------------------------------------
    void GameObject::generateNshowdefaultgraph() {
        double battleWeight = 1.7;
        double shopWeight = 1.0;
        double specialWeight = 1.0;

        Graph g = generateRoguelikeGraph(
            5,          // NodePathLen
            2, 4,       // minNodesPerLayer, maxNodesPerLayer
            battleWeight, shopWeight, specialWeight,
            3, 2        // maxOutgoing, maxExtra
            // seed не указан → используется time(NULL)
        );
        g.print();
    }

} // namespace game