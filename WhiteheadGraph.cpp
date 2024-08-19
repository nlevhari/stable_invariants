#include "WhiteheadGraph.h"
#include <stack>

WhiteheadGraph::WhiteheadGraph(int r) : CoreWhiteheadGraph(r) {}

void WhiteheadGraph::removeVertex(int vertex) {
    edges.erase(vertex);
    for (auto& pair : edges) {
        pair.second.remove_if([vertex](const std::pair<int, int>& edge) {
            return edge.first == vertex || edge.second == vertex;
        });
    }
}

void WhiteheadGraph::rememberOriginalVertex(int originalVertex, int newVertex) {
    originalVertices[newVertex] = originalVertex;
}

const std::unordered_map<int, int>& WhiteheadGraph::getOriginalVertices() const {
    return originalVertices;
}

bool WhiteheadGraph::hasMinimumDegree(int minDegree) const {
    for (const auto& pair : edges) {
        if (pair.second.size() < minDegree) {
            return false;
        }
    }
    return true;
}

bool WhiteheadGraph::isConnected() const {
    if (edges.empty()) return false;

    std::unordered_map<int, bool> visited;
    std::list<int> queue;

    int startVertex = edges.begin()->first;
    queue.push_back(startVertex);
    visited[startVertex] = true;

    while (!queue.empty()) {
        int currentVertex = queue.front();
        queue.pop_front();

        for (const auto& edge : edges.at(currentVertex)) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push_back(neighbor);
            }
        }
    }

    for (const auto& pair : edges) {
        if (!visited[pair.first]) {
            return false;
        }
    }
    return true;
}

// Internal DFS utility function
void WhiteheadGraph::internalDFS(int vertex, std::unordered_map<int, bool>& visited,
                                 const std::function<void(int)>& processVertex) const {
    std::stack<int> stack;
    stack.push(vertex);

    while (!stack.empty()) {
        int currentVertex = stack.top();
        stack.pop();

        if (!visited[currentVertex]) {
            visited[currentVertex] = true;
            processVertex(currentVertex);

            for (const auto& edge : edges.at(currentVertex)) {
                int neighbor = edge.first;
                if (!visited[neighbor]) {
                    stack.push(neighbor);
                }
            }
        }
    }
}

bool WhiteheadGraph::isBiconnected() const {
    if (edges.empty()) return true;

    std::unordered_map<int, int> disc;
    std::unordered_map<int, int> low;
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> parentMap;
    int time = 0;
    bool isBiconnected = true;

    std::function<void(int)> processVertex = [&](int vertex) {
        int children = 0;
        visited[vertex] = true;
        disc[vertex] = low[vertex] = ++time;

        for (const auto& edge : edges.at(vertex)) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                children++;
                parentMap[neighbor] = vertex;
                internalDFS(neighbor, visited, processVertex);

                low[vertex] = std::min(low[vertex], low[neighbor]);

                if (parentMap[vertex] == -1 && children > 1)
                    isBiconnected = false;

                if (parentMap[vertex] != -1 && low[neighbor] >= disc[vertex])
                    isBiconnected = false;
            } else if (neighbor != parentMap[vertex]) {
                low[vertex] = std::min(low[vertex], disc[neighbor]);
            }
        }
    };

    int startVertex = edges.begin()->first;
    internalDFS(startVertex, visited, processVertex);

    for (const auto& pair : edges) {
        if (!visited[pair.first]) {
            isBiconnected = false;
            break;
        }
    }

    return isBiconnected;
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



bool WhiteheadGraph::isValid(const bool partitioned=false) const {
    return getEdges().size() >= 2 &&
            hasMinimumDegree(2) &&
            isConnected() &&
            (!partitioned || isBiconnected());
}
