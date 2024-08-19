#ifndef WHITEHEAD_GRAPH_H
#define WHITEHEAD_GRAPH_H

#include "CoreWhiteheadGraph.h"
#include <unordered_map>

class WhiteheadGraph : public CoreWhiteheadGraph {
public:
    WhiteheadGraph(int r);
    void removeVertex(int vertex);
    void rememberOriginalVertex(int originalVertex, int newVertex);
    const std::unordered_map<int, int>& getOriginalVertices() const;

    bool hasMinimumDegree(int minDegree) const;
    bool isConnected() const;
    bool isBiconnected() const;
    bool isValid(const bool partitioned=false) const;
    std::vector<WhiteheadGraph> getConnectedComponents() const;

private:
    std::unordered_map<int, int> originalVertices;

    void internalDFS(int vertex, std::unordered_map<int, bool>& visited,
                     const std::function<void(int)>& processVertex) const;
};

#endif // WHITEHEAD_GRAPH_H
