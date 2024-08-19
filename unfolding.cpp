#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include "WhiteheadGraph.h"

// Helper function to generate all valid partitions
void generatePartitionsHelper(const std::vector<std::pair<int, int>>& edges,
                              std::vector<std::vector<std::pair<int, int>>>& currentPartition,
                              std::vector<std::vector<std::vector<std::pair<int, int>>>>& allPartitions,
                              int index) {
    if (index == edges.size()) {
        allPartitions.push_back(currentPartition);
        return;
    }

    // Try to add the current edge to existing subsets
    for (auto& subset : currentPartition) {
        if (subset.size() < edges.size() - 1) { // Ensure size at least 2 later
            subset.push_back(edges[index]);
            generatePartitionsHelper(edges, currentPartition, allPartitions, index + 1);
            subset.pop_back();
        }
    }

    // Try to create a new subset with the current edge
    if (currentPartition.size() < edges.size() / 2) { // Prevent creating too many single-edge subsets
        currentPartition.push_back({edges[index]});
        generatePartitionsHelper(edges, currentPartition, allPartitions, index + 1);
        currentPartition.pop_back();
    }
}

// Wrapper function to generate all valid partitions
std::vector<std::vector<std::vector<std::pair<int, int>>>> generatePartitions(const std::list<std::pair<int, int>>& edges) {
    std::vector<std::vector<std::vector<std::pair<int, int>>>> partitions;
    std::vector<std::vector<std::pair<int, int>>> currentPartition;

    std::vector<std::pair<int, int>> edgeVec(edges.begin(), edges.end());
    generatePartitionsHelper(edgeVec, currentPartition, partitions, 0);

    // Filter out partitions with subsets smaller than 2
    partitions.erase(std::remove_if(partitions.begin(), partitions.end(),
        [](const std::vector<std::vector<std::pair<int, int>>>& partition) {
            return std::any_of(partition.begin(), partition.end(), [](const std::vector<std::pair<int, int>>& subset) {
                return subset.size() < 2;
            });
        }), partitions.end());

    return partitions;
}

// Function to replace vertex i in the subgraph with new vertices corresponding to the edge subsets
std::vector<WhiteheadGraph> partitionAndReplaceVertices(const WhiteheadGraph& subgraph) {
    std::vector<WhiteheadGraph> newGraphs;

    // For each vertex in the subgraph, perform the partitioning and create new graphs
    for (const auto& vertexEdges : subgraph.getEdges()) {
        int vertex = vertexEdges.first;
        const std::list<std::pair<int, int>>& edges = vertexEdges.second;

        // Generate partitions of the edges incident to this vertex
        std::vector<std::vector<std::vector<std::pair<int, int>>>> partitions = generatePartitions(edges);

        for (const auto& partition : partitions) {
            WhiteheadGraph newGraph = subgraph;

            // Remove the original vertex and its edges
            newGraph.removeVertex(vertex);

            // Add new vertices for each subset in the partition
            int newVertexId = vertex * 10; // Assuming this creates a new unique vertex ID
            for (const auto& edgeSet : partition) {
                for (const auto& edge : edgeSet) {
                    newGraph.addEdge(newVertexId, edge.first, edge.second);
                }
                newVertexId++;
            }

            // Remember the original vertex
            newGraph.rememberOriginalVertex(vertex, newVertexId - 1);
            
            newGraphs.push_back(newGraph);
        }
    }

    return newGraphs;
}

// Function to filter the valid WhiteheadGraph objects that are visibly irreducible
std::vector<WhiteheadGraph> filterValidWhiteheadGraphsFromPartitions(
    const std::vector<WhiteheadGraph>& graphs) {

    std::vector<WhiteheadGraph> filteredGraphs;

    for (const auto& graph : graphs) {
        bool isValid = true; // Initialize bool per graph
        for (const auto& component : graph.getConnectedComponents()){
            isValid = isValid && component.isValid(true); // Check if all components are valid
        }
        if (isValid) {
            filteredGraphs.push_back(graph);
        }
    }

    return filteredGraphs;
}
