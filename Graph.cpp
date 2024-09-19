#include <algorithm>
#include <stack>
#include <utility>
#include <limits>
#include <iostream>
#include <cassert>
#include "Graph.h"

Graph::Graph(const std::vector<int> vertices_) : vertices(vertices_) { }

void Graph::addEdge(int start, int end, int position){
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
    std::vector<std::pair<int, int>> incomingEdges = {};

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

const std::pair<std::vector<int>, std::vector<int>> Graph::getNeighborVecsAtVertex(int vertex) const {
    const auto& incoming_edges = getIncomingEdges(vertex); 
    std::vector<std::pair<int, int>> outgoing_edges = {}; 
    if (edges.find(vertex) != edges.end()){ 
       outgoing_edges = edges.at(vertex); 
    };
    std::vector<int> incoming = {};
    for (const auto& edge: incoming_edges){
        incoming.push_back(edge.first);
    }
    std::vector<int> outgoing = {};
    for (const auto& edge: outgoing_edges){
        outgoing.push_back(edge.first);
    }
    const auto& neighbors_pair = std::make_pair(incoming, outgoing);
    return neighbors_pair; 
}

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

        const auto& neighbors = getNeighborsAtVertex(vertex);
        for (const int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                queue.push_back(neighbor);
            }
        }
    }

    // Check if all vertices were visited
    return visited.size() == vertices.size();
}

const std::vector<int> Graph::getNeighborsAtVertex(int vertex) const{
    const auto& incoming_outgoing = getNeighborVecsAtVertex(vertex);
    std::vector<int> neighbor_vec;
    neighbor_vec.reserve(incoming_outgoing.first.size() + incoming_outgoing.second.size());
    neighbor_vec.insert(neighbor_vec.end(), incoming_outgoing.second.begin(), incoming_outgoing.second.end());
    neighbor_vec.insert(neighbor_vec.end(), incoming_outgoing.first.begin(), incoming_outgoing.first.end());
    return neighbor_vec;
}

void Graph::internalDFS(int vertex, std::unordered_map<int, bool>& visited,
                        std::unordered_map<int, int>& disc, std::unordered_map<int, int>& low,
                        std::unordered_map<int, int>& parentMap, std::unordered_set<int>& articulationPoints,
                        int& time, bool& isBiconnected) const {
    int children = 0;
    visited[vertex] = true;
    disc[vertex] = low[vertex] = ++time;
    
    for (const int& neighbor : getNeighborsAtVertex(vertex)) {
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

bool Graph::isValidWHGraph(const bool partitioned) const {
    const bool enough_edges = getEdges().size() >= 2;
    if (!enough_edges) return false;
    const bool at_least_deg_2 = hasMinimumDegree(2);
    if (!at_least_deg_2) return false;
    const bool connected = isConnected();
    if (!connected) return false;
    const bool not_partition_or_biconnected = !partitioned || isBiconnected();
    return not_partition_or_biconnected;
}

std::vector<Graph> Graph::getConnectedComponents() const {
    std::unordered_set<int> visited;
    std::vector<Graph> components;

    auto processVertexForComponents = [&](int vertex) {
        std::vector<std::pair<int, int>> edges_for_component;
        std::list<int> queue;
        queue.push_back(vertex);
        visited.insert(vertex);
        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop_front();
            const auto& incoming_outgoing = getNeighborVecsAtVertex(currentVertex);
            auto processNeighbor = [&] (int neighbor, bool incoming){
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push_back(neighbor);
                    edges_for_component.push_back(incoming ? std::make_pair(neighbor, currentVertex) : std::make_pair(currentVertex, neighbor));
                }
            };
            for (const auto& nbr: incoming_outgoing.first){
                processNeighbor(nbr, true);
            }
            for (const auto& nbr: incoming_outgoing.second){
                processNeighbor(nbr, false);
            }
        }

        std::unordered_set<int> componentVertices;
        for (const auto& edge : edges_for_component) {
            componentVertices.insert(edge.first);
            componentVertices.insert(edge.second);
        }

        Graph componentGraph(std::vector<int>(componentVertices.begin(), componentVertices.end()));
        for (const auto& edge : edges_for_component) {
            componentGraph.addEdge(edge.first, edge.second, 0); // 0 is a fake position
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