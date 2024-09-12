// Graph.h
#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <utility>
#include <functional>

struct Edge {
    int start;
    int end;
    int position;
};

class Graph {
public:
    Graph(const std::vector<int> vertices_);

    void addEdge(int start, int end, int position);
    std::vector<Edge> getAllEdges() const;
    const std::vector<int> getVertices() const { return vertices; }
    void retainVertices(const std::unordered_set<int>& activeVertices);
    const std::unordered_map<int, std::vector<std::pair<int, int>>>& getEdges() const {return edges;}
    const std::unordered_map<int, std::pair<int, int>> getEdgesByPosition() const;
    void displayGraph() const;
    std::vector<std::pair<int, int>> getIncomingEdges(int vertex) const;
    void removeVertex(int vertex);
    void addVertex(int vertex);
    const std::unordered_map<int, int>& getOriginalVertices() const;
    void rememberOriginalVertex(int originalVertex, int newVertex);
    bool hasMinimumDegree(int minDegree) const;
    bool isConnected() const;
    bool isBiconnected() const;

protected:
    std::vector<int> vertices;
    std::unordered_map<int, std::vector<std::pair<int, int> > > edges;
    std::unordered_map<int, int> originalVertices;
    void internalDFS(int vertex, std::unordered_map<int, bool>& visited,
                        std::unordered_map<int, int>& disc, std::unordered_map<int, int>& low,
                        std::unordered_map<int, int>& parentMap, std::unordered_set<int>& articulationPoints,
                        int& time, bool& isBiconnected) const;
};

#endif // GRAPH_H
