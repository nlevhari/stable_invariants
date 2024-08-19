// CoreWhiteheadGraph.h
#ifndef COREWHITEHEADGRAPH_H
#define COREWHITEHEADGRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <utility>

struct Edge {
    int start;
    int end;
    int position;
};

class CoreWhiteheadGraph {
public:
    CoreWhiteheadGraph(int r);

    void addEdge(int start, int end, int position);
    int getR() const;
    std::vector<Edge> getAllEdges() const;
    void retainVertices(const std::unordered_set<int>& activeVertices);
    const std::unordered_map<int, std::list<std::pair<int, int>>>& getEdges() const {
        return edges;
    }
    void displayGraph() const;

protected:
    int r;
    std::vector<int> vertices;
    std::unordered_map<int, std::list<std::pair<int, int> > > edges;
};

CoreWhiteheadGraph constructCoreWhiteheadGraph(const std::vector<int>& word, int r);

class WhiteheadGraph : public CoreWhiteheadGraph{
    void removeVertex(int vertex);
    void rememberOriginalVertex(int original, int newVertex);
};

#endif // COREWHITEHEADGRAPH_H
