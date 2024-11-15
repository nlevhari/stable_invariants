#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include "WhiteheadGraph.h"
#include "linear_program_construction.h"
#include "stable_invariants_types.h"
#include "utils.h"
#include <cassert>
#include <iostream>
#include <sstream>

namespace VariableConstruction {
    
    // Function to filter the valid WhiteheadGraph objects that are visibly irreducible
    std::vector<WhiteheadGraph> filterValidWhiteheadGraphsFromPartitions(const std::vector<WhiteheadGraph>& graphs, StableInvariant& invariant) {

        std::vector<WhiteheadGraph> filteredGraphs;

        for (const auto& graph : graphs) {
            bool isValid = true; // Initialize bool per graph
            for (const auto& component : graph.getConnectedComponents()){
                isValid = isValid && invariant.isValidWHGraphForInvariant(component);
            }
            if (isValid) {
                filteredGraphs.push_back(graph);
            }
        }

        return filteredGraphs;
    }

    std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> generatePartitionCombinations(
        const std::vector<int> vertices,
        const std::unordered_map<int, std::vector<int>>& outgoingEdges,
        const std::unordered_map<int, std::vector<int>>& incomingEdges) {
        
        // Vector to store all the partition combinations for each vertex
        std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> allCombinations;

        // Helper function to generate combinations recursively
        std::function<void(int, std::unordered_map<int, std::vector<std::vector<int>>>&)> generate =
            [&](int index, std::unordered_map<int, std::vector<std::vector<int>>>& currentCombination) {
                if (index == vertices.size()) {
                    // Base case: if we've processed all vertices, add the current combination to the list
                    allCombinations.push_back(currentCombination);
                    return;
                }

                int vertex = vertices[index];
                auto it = outgoingEdges.find(vertex);
                std::vector<int> edges_at_vertex = (it != outgoingEdges.end()) ? it->second: std::vector<int>();
                it = incomingEdges.find(vertex);
                std::vector<int> incoming = (it != incomingEdges.end()) ? it->second: std::vector<int>();
                for (size_t i=0; i<incoming.size(); i++){
                    incoming[i] = -incoming[i];
                }
                edges_at_vertex.insert(edges_at_vertex.end(), incoming.begin(), incoming.end());

                // Get all possible partitions for outgoing and incoming edges
                std::vector<std::vector<std::vector<int>>> partitions = Utils::generatePartitions(edges_at_vertex);

                // Iterate over all combinations of outgoing and incoming partitions
                for (const std::vector<std::vector<int>>& partition : partitions) {
                    // Store the partition in the current combination
                    currentCombination[vertex] = partition;

                    // Recur to process the next vertex
                    generate(index + 1, currentCombination);

                    // Backtrack: remove the partition for the current vertex to explore other combinations
                    currentCombination.erase(vertex);
                }
            };

        // Initialize an empty current combination and start the recursive generation
        std::unordered_map<int, std::vector<std::vector<int>>> currentCombination;
        generate(0, currentCombination);

        return allCombinations;
    }

    std::vector<WhiteheadGraph> partitionAndReplaceVertices(const WhiteheadGraph& originalGraph) {
        std::vector<WhiteheadGraph> newGraphs;

        // Step 1: Generate partitions for each vertex considering incoming and outgoing edges
        std::unordered_map<int, std::vector<int>> outgoing_vertex_to_edge_positions;
        std::unordered_map<int, std::vector<int>> incoming_vertex_to_edge_positions;
        for (const auto& vertexEdges: originalGraph.getEdges()) {
            for (const std::pair<int,int> incoming_vertex_and_position: vertexEdges.second) {
                outgoing_vertex_to_edge_positions[vertexEdges.first].push_back(incoming_vertex_and_position.second);
                incoming_vertex_to_edge_positions[incoming_vertex_and_position.first].push_back(incoming_vertex_and_position.second);
            }
        }

        std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> partitionCombinations = generatePartitionCombinations(originalGraph.getVertices(), outgoing_vertex_to_edge_positions, incoming_vertex_to_edge_positions);

        // Step 3: Construct new graphs based on each partition combination
        for (const auto& partitionChoice : partitionCombinations) {
            WhiteheadGraph newGraph(0, true);
            std::unordered_map<int, int> position_to_new_source;
            std::unordered_map<int, int> position_to_new_target;

            // Add new vertices and track original vertices
            int newVertexId = 0;
            bool partition_invalid = false;
            for (const auto& vertexPartition : partitionChoice) {
                if (partition_invalid) { break; }
                int originalVertex = vertexPartition.first;
                const auto& subsets = vertexPartition.second;
                for (const auto& subset : subsets) {
                    if (subset.size() < 2) {partition_invalid = true; break; }
                    newGraph.addVertex(newVertexId);
                    newGraph.rememberOriginalVertex(originalVertex, newVertexId);
                    for (const int position: subset){
                        assert(position!=0 && "bug - somehow position=0"); 
                        if (position > 0){
                            position_to_new_source[position] = newVertexId;
                        } else {
                            position_to_new_target[-position] = newVertexId;
                        }
                    }
                    newVertexId++;
                }
            }
            if (partition_invalid) { continue; } else {
                const bool verbose = false;
                if (verbose) {
                    for (const auto& vertexPartition : partitionChoice) {
                        std::cout<<"partition for vertex: "<<vertexPartition.first<<"\n subsets: \n";
                        const auto& subsets = vertexPartition.second;
                        int i=0;
                        for (const auto& subset : subsets) {
                            i++;
                            std::cout<<"subset "<<i<<": "<<"\n";
                            for (const int position: subset){
                                std::cout<<position<<",\t";
                            }
                            std::cout<<"\n";
                        }
                    }
                }

            }
            for (const auto& position_and_source : position_to_new_source) {
                int targetId = position_to_new_target[position_and_source.first];
                newGraph.addEdge(position_and_source.second, targetId, position_and_source.first);
            }

            newGraphs.push_back(newGraph);
        }

        return newGraphs;
    }
}