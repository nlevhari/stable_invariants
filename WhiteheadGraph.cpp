#include "WhiteheadGraph.h"
#include <stack>
#include <iostream>
#include <sstream>

const std::vector<int> generateVerticesFromR(const int r){
    std::vector<int> vertices;
    for (int i = 0; i < r; ++i) {
        vertices.push_back(i + 1);
        vertices.push_back(-(i + 1));
    };
    return vertices;
}

WhiteheadGraph::WhiteheadGraph(int r) : Graph(generateVerticesFromR(r)) {}

const std::vector<int> generate_s_vertices(const int s){
    std::vector<int> vertices;
    for (int i = 0; i < s; ++i) {
        vertices.push_back(i);
    };
    return vertices;
}

WhiteheadGraph::WhiteheadGraph(int s, bool not_core) : Graph(generate_s_vertices(s)) {
    assert(not_core && "bad instantiation for WHG"); // if not_core=false, bad instantiation.
}

std::vector<WhiteheadGraph> WhiteheadGraph::getConnectedComponents() const {
    std::unordered_map<int, bool> visited;
    std::vector<WhiteheadGraph> components;
    int r = this->getOriginalVertices().size() / 2;

    auto processVertexForComponents = [&](int vertex) {
        WhiteheadGraph component(r);
        std::list<int> queue;
        queue.push_back(vertex);

        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop_front();

            if (!visited[currentVertex]) {
                visited[currentVertex] = true;

                // Add vertex to component
                for (const auto& edge : edges.at(currentVertex)) {
                    int neighbor = edge.first;
                    component.addEdge(currentVertex, neighbor, edge.second);
                    if (!visited[neighbor]) {
                        queue.push_back(neighbor);
                    }
                }
            }
        }
        components.push_back(component);
    };

    for (const auto& pair : edges) {
        int vertex = pair.first;
        if (!visited[vertex]) {
            internalDFS(vertex, visited, processVertexForComponents);
        }
    }

    return components;
}



bool WhiteheadGraph::isValid(const bool partitioned) const {
    return getEdges().size() >= 2 &&
            hasMinimumDegree(2) &&
            isConnected() &&
            (!partitioned || isBiconnected());
}
