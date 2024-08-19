#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <unordered_set>
#include "WhiteheadGraph.h"

// Helper function to generate all subsets of a set of edges
std::vector<std::vector<Edge>> generateSubsets(const std::vector<Edge>& edges) {
    std::vector<std::vector<Edge>> subsets;
    int n = edges.size();
    int num_subsets = 1 << n; // 2^n subsets

    for (int i = 0; i < num_subsets; ++i) {
        std::vector<Edge> subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(edges[j]);
            }
        }
        subsets.push_back(subset);
    }

    return subsets;
}

// Helper function to build a subgraph induced by a subset of edges
WhiteheadGraph buildSubgraph(const CoreWhiteheadGraph& graph, const std::vector<Edge>& subset) {
    // Create a new subgraph
    WhiteheadGraph subgraph(graph.getR());
    std::unordered_set<int> activeVertices;

    // Add the edges to the subgraph and track active vertices
    for (const Edge& edge : subset) {
        subgraph.addEdge(edge.start, edge.end, edge.position);
        activeVertices.insert(edge.start);
        activeVertices.insert(edge.end);
    }

    // Remove any vertices not in the activeVertices set
    subgraph.retainVertices(activeVertices);

    return subgraph;
}

// Main function to find all subgraphs with minimum vertex degree 2
std::vector<WhiteheadGraph> findValidSubgraphsFromCore(const CoreWhiteheadGraph& graph) {
    std::vector<WhiteheadGraph> validSubgraphs;

    // Generate all subsets of the edge set
    std::vector<Edge> edges = graph.getAllEdges();
    std::vector<std::vector<Edge>> subsets = generateSubsets(edges);

    // For each subset, build the subgraph and check its minimum vertex degree
    for (const std::vector<Edge>& subset : subsets) {
        WhiteheadGraph subgraph = buildSubgraph(graph, subset);
        if (!subset.empty() && subgraph.isValid(false)) {
            validSubgraphs.push_back(subgraph);
        }
    }

    return validSubgraphs;
}
