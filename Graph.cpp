#include <algorithm>
#include <stack>
#include <utility>
#include <limits>
#include <iostream>
#include <cassert>
#include "Graph.h"

Graph::Graph(const std::vector<int> vertices_) : vertices(vertices_) { }

void Graph::addEdge(int start, int end, int position){
    assert(position!=0 && "We don't allow position=0 in this program.");
    edges[start].push_back(std::make_pair(end, position));
}

std::vector<Edge> Graph::getAllEdges() const {
    std::vector<Edge> allEdges;
    for (const auto& vertex : edges) {
        for (const auto& edge : vertex.second) {
            allEdges.push_back({vertex.first, edge.first, edge.second});
        }
    }
    return allEdges;
}

const std::unordered_map<int, std::pair<int, int>> Graph::getEdgesByPosition() const{
    std::unordered_map<int, std::pair<int, int>> edges_by_position;
    for (const auto& out_vertex_to_edges: edges){
        for (const auto& edge: out_vertex_to_edges.second){
            edges_by_position[edge.second] = std::make_pair(out_vertex_to_edges.first, edge.first);
        }
    }
    return edges_by_position;
}

void Graph::retainVertices(const std::unordered_set<int>& activeVertices) {
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

void Graph::displayGraph() const {
    for (const auto& vertex : edges) {
        std::cout << "Vertex " << vertex.first << " has edges to: ";
        for (const auto& edge : vertex.second) {
            std::cout << "(" << edge.first << ", position: " << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::pair<int, int>> Graph::getIncomingEdges(int vertex) const {
    std::vector<std::pair<int, int>> incomingEdges;

    for (const auto& edgeList : edges) {
        int startVertex = edgeList.first;
        for (const auto& edge: edgeList.second){
            int endVertex = edge.first;

            // Check if this edge is incoming to the given vertex
            if (endVertex == vertex) {
                incomingEdges.push_back({startVertex, endVertex});
            }
        }
    }

    return incomingEdges;
}

void Graph::removeVertex(int vertex) {
    edges.erase(vertex);
    for (auto& pair : edges) {
        auto removeCondition = [vertex](const std::pair<int, int>& edge) {
            return edge.first == vertex || edge.second == vertex;
        };
        auto it = std::remove_if(pair.second.begin(), pair.second.end(), removeCondition);
        pair.second.erase(it, pair.second.end());
    }
}

void Graph::addVertex(int vertex){
    vertices.push_back(vertex);
}

void Graph::rememberOriginalVertex(int originalVertex, int newVertex) {
    originalVertices[newVertex] = originalVertex;
}

const std::unordered_map<int, int>& Graph::getOriginalVertices() const {
    return originalVertices;
}

bool Graph::hasMinimumDegree(int minDegree) const {
    std::unordered_map<int, int> vertexDegree;

    // Iterate over all edges in the graph
    for (const auto& vertexEdges : edges) {
        int startVertex = vertexEdges.first;

        // Increase degree count for the start vertex
        vertexDegree[startVertex] += vertexEdges.second.size();

        // Increase degree count for the end vertices
        for (const auto& edge : vertexEdges.second) {
            int endVertex = edge.first;
            vertexDegree[endVertex] += 1;
        }
    }

    // Check if all vertices have at least the minimum degree
    for (const auto& degree : vertexDegree) {
        if (degree.second < minDegree) {
            return false;
        }
    }

    return true;
}

// bool Graph::isConnected() const {
//     if (edges.empty()) return false;

//     std::unordered_set<int> visited;
//     std::list<int> queue;

//     int startVertex = edges.begin()->first;
//     queue.push_back(startVertex);
//     visited.insert(startVertex);

//     while (!queue.empty()) {
//         int currentVertex = queue.front();
//         queue.pop_front();

//         for (const auto& edge : edges.at(currentVertex)) {
//             int neighbor = edge.first;
//             if (visited.find(neighbor) == visited.end()) {
//                 visited.insert(neighbor);
//                 queue.push_back(neighbor);
//             }
//         }
//     }

//     for (const auto& pair : edges) {
//         if (visited.find(pair.first) == visited.end()) {
//             return false;
//         }
//     }
//     return true;
// }

// // Internal DFS utility function
// void Graph::internalDFS(int vertex, std::unordered_set<int>& visited,
//                                  const std::function<void(int)>& processVertex) const {
//     std::stack<int> stack;
//     stack.push(vertex);

//     while (!stack.empty()) {
//         int currentVertex = stack.top();
//         stack.pop();

//         if (visited.find(currentVertex) == visited.end()) {
//             processVertex(currentVertex);

//             for (const auto& edge : edges.at(currentVertex)) {
//                 int neighbor = edge.first;
//                 if (visited.find(neighbor) == visited.end()) {
//                     stack.push(neighbor);
//                 }
//             }
//         }
//     }
// }

// bool Graph::isBiconnected() const {
//     if (edges.empty()) return true;

//     std::unordered_map<int, int> disc;
//     std::unordered_map<int, int> low;
//     std::unordered_set<int> visited;
//     std::unordered_map<int, int> parentMap;
//     int time = 0;
//     bool isBiconnected = true;

//     std::function<void(int)> processVertex = [&](int vertex) {
//         int children = 0;
//         visited.insert(vertex);
//         disc[vertex] = low[vertex] = ++time;

//         for (const auto& edge : edges.at(vertex)) {
//             int neighbor = edge.first;
//             if (visited.find(neighbor) == visited.end()) {
//                 children++;
//                 parentMap[neighbor] = vertex;
//                 internalDFS(neighbor, visited, processVertex);

//                 low[vertex] = std::min(low[vertex], low[neighbor]);

//                 if (parentMap[vertex] == -1 && children > 1)
//                     isBiconnected = false;

//                 if (parentMap[vertex] != -1 && low[neighbor] >= disc[vertex])
//                     isBiconnected = false;
//             } else if (neighbor != parentMap[vertex]) {
//                 low[vertex] = std::min(low[vertex], disc[neighbor]);
//             }
//         }
//     };

//     int startVertex = edges.begin()->first;
//     internalDFS(startVertex, visited, processVertex);

//     for (const auto& pair : edges) {
//         if (visited.find(pair.first) == visited.end()) {
//             isBiconnected = false;
//             break;
//         }
//     }

//     return isBiconnected;
// }

bool Graph::isConnected() const {
    if (vertices.empty()) return true; // An empty graph is considered connected

    std::unordered_set<int> visited;
    std::list<int> queue;
    int startVertex = vertices.front();

    // Start BFS or DFS from the first vertex
    queue.push_back(startVertex);
    visited.insert(startVertex);

    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop_front();

        for (const auto& edge : edges.at(vertex)) {
            int neighbor = edge.first;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push_back(neighbor);
            }
        }
    }

    // Check if all vertices were visited
    return visited.size() == vertices.size();
}

void Graph::internalDFS(int vertex, std::unordered_map<int, bool>& visited,
                        std::unordered_map<int, int>& disc, std::unordered_map<int, int>& low,
                        std::unordered_map<int, int>& parentMap, std::unordered_set<int>& articulationPoints,
                        int& time, bool& isBiconnected) const {
    int children = 0;
    visited[vertex] = true;
    disc[vertex] = low[vertex] = ++time;

    for (const auto& edge : edges.at(vertex)) {
        int neighbor = edge.first;

        if (!visited[neighbor]) {
            children++;
            parentMap[neighbor] = vertex;
            internalDFS(neighbor, visited, disc, low, parentMap, articulationPoints, time, isBiconnected);

            low[vertex] = std::min(low[vertex], low[neighbor]);

            if (parentMap[vertex] == -1 && children > 1) {
                articulationPoints.insert(vertex);
            }

            if (parentMap[vertex] != -1 && low[neighbor] >= disc[vertex]) {
                articulationPoints.insert(vertex);
            }
        } else if (neighbor != parentMap[vertex]) {
            low[vertex] = std::min(low[vertex], disc[neighbor]);
        }
    }
}

bool Graph::isBiconnected() const {
    if (!isConnected()) return false;

    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> disc;
    std::unordered_map<int, int> low;
    std::unordered_map<int, int> parentMap;
    std::unordered_set<int> articulationPoints;
    int time = 0;
    bool isBiconnected = true;

    for (int vertex : vertices) {
        visited[vertex] = false;
        parentMap[vertex] = -1;
    }

    for (int vertex : vertices) {
        if (!visited[vertex]) {
            internalDFS(vertex, visited, disc, low, parentMap, articulationPoints, time, isBiconnected);
        }
    }

    return articulationPoints.empty();
}
