// CoreWhiteheadGraph.cpp
#include "CoreWhiteheadGraph.h"
#include <iostream>

CoreWhiteheadGraph::CoreWhiteheadGraph(int r) : r(r) {
    for (int i = 0; i < r; ++i) {
        vertices.push_back(i + 1);
        vertices.push_back(-(i + 1));
    }
}

void CoreWhiteheadGraph::addEdge(int start, int end, int position) {
    edges[start].push_back(std::make_pair(end, position));
}

int CoreWhiteheadGraph::getR() const {
    return r;
}

std::vector<Edge> CoreWhiteheadGraph::getAllEdges() const {
    std::vector<Edge> allEdges;
    for (const auto& vertex : edges) {
        for (const auto& edge : vertex.second) {
            allEdges.push_back({vertex.first, edge.first, edge.second});
        }
    }
    return allEdges;
}

void CoreWhiteheadGraph::retainVertices(const std::unordered_set<int>& activeVertices) {
    // Remove edges that are not incident to active vertices
    for (auto it = edges.begin(); it != edges.end();) {
        if (activeVertices.find(it->first) == activeVertices.end()) {
            it = edges.erase(it);
        } else {
            for (auto edgeIt = it->second.begin(); edgeIt != it->second.end();) {
                if (activeVertices.find(edgeIt->first) == activeVertices.end()) {
                    edgeIt = it->second.erase(edgeIt);
                } else {
                    ++edgeIt;
                }
            }
            ++it;
        }
    }
}

void CoreWhiteheadGraph::displayGraph() const {
    for (const auto& vertex : edges) {
        std::cout << "Vertex " << vertex.first << " has edges to: ";
        for (const auto& edge : vertex.second) {
            std::cout << "(" << edge.first << ", position: " << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

CoreWhiteheadGraph constructCoreWhiteheadGraph(const std::vector<int>& word, int r) {
    CoreWhiteheadGraph graph(r);

    int n = word.size();
    for (int i = 0; i < n; ++i) {
        int k = word[i];
        int l = word[(i + 1) % n];

        int start = (k < 0) ? -k : -k;
        int end = (l < 0) ? l : l;

        graph.addEdge(start, end, i);
    }

    return graph;
}