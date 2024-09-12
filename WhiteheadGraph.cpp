#include "WhiteheadGraph.h"
#include "Graph.h"
#include <stack>
#include <cassert>
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

WhiteheadGraph::WhiteheadGraph(Graph G_) : Graph(G_) {}

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

WhiteheadGraph::WhiteheadGraph(const std::vector<int> vertices_) : Graph(vertices_) { }

std::vector<WhiteheadGraph> WhiteheadGraph::getConnectedComponents() const {
    std::unordered_set<int> visited;
    std::vector<WhiteheadGraph> components;

    auto processVertexForComponents = [&](int vertex) {
        std::unordered_map<int, std::pair<int, int>> edges_for_component;
        std::list<int> queue;
        queue.push_back(vertex);
        visited.insert(vertex);

        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop_front();

            for (const auto& edge : edges.at(currentVertex)) {
                int neighbor = edge.first;
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push_back(neighbor);
                }
                edges_for_component.insert({currentVertex, edge});
            }
        }

        std::vector<int> componentVertices;
        for (const auto& edge : edges_for_component) {
            if (std::find(componentVertices.begin(), componentVertices.end(), edge.first) == componentVertices.end()) {
                componentVertices.push_back(edge.first);
            }
        }

        WhiteheadGraph componentGraph(componentVertices);
        for (const auto& edge : edges_for_component) {
            componentGraph.addEdge(edge.first, edge.second.first, edge.second.second);
        }

        components.push_back(componentGraph);
    };

    for (int vertex : vertices) {
        if (visited.find(vertex) == visited.end()) {
            processVertexForComponents(vertex);
        }
    }

    return components;
}


// std::vector<WhiteheadGraph> WhiteheadGraph::getConnectedComponents() const {
//     std::unordered_set<int> visited;
//     std::vector<WhiteheadGraph> components;

//     auto processVertexForComponents = [&](int vertex) {
//         std::unordered_map<int, std::unordered_map<int, int>> edges_for_component;
//         std::list<int> queue;
//         queue.push_back(vertex);

//         while (!queue.empty()) {
//             int currentVertex = queue.front();
//             queue.pop_front();

//             if (visited.find(currentVertex) == visited.end()) {
//                 visited.insert(currentVertex);

//                 for (const auto& edge : edges.at(currentVertex)) {
//                     int neighbor = edge.first;
//                     int position = edge.second;
//                     edges_for_component[currentVertex][neighbor] = position;

//                     if (visited.find(neighbor) == visited.end()) {
//                         queue.push_back(neighbor);
//                     }
//                 }
//             }
//         }

//         std::unordered_set<int> vertices_for_component;
//         for (const auto& edgeMap : edges_for_component) {
//             vertices_for_component.insert(edgeMap.first);
//             for (const auto& edge : edgeMap.second) {
//                 vertices_for_component.insert(edge.first);
//             }
//         }
//         if (vertices_for_component.size() == 0){
//             return;
//         }
//         auto component = WhiteheadGraph(std::vector<int>(vertices_for_component.begin(), vertices_for_component.end()));
//         for (const auto& edgeMap : edges_for_component) {
//             int from = edgeMap.first;
//             for (const auto& edge : edgeMap.second) {
//                 int to = edge.first;
//                 int position = edge.second;
//                 component.addEdge(from, to, position);
//             }
//         }
//         components.push_back(component);
//     };

//     for (const auto& pair : edges) {
//         int vertex = pair.first;
//         if (visited.find(vertex) == visited.end()) {
//             internalDFS(vertex, visited, processVertexForComponents);
//         }
//     }

//     return components;
// }




bool WhiteheadGraph::isValid(const bool partitioned) const {
    const bool enough_edges = getEdges().size() >= 2;
    if (!enough_edges) return false;
    const bool at_least_deg_2 = hasMinimumDegree(2);
    if (!at_least_deg_2) return false;
    const bool connected = isConnected();
    if (!connected) return false;
    const bool not_partition_or_biconnected = !partitioned || isBiconnected();
    return not_partition_or_biconnected;
}
