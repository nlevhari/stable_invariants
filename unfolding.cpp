#include <vector>
#include <unordered_map>
#include <list>
#include <algorithm>
#include "WhiteheadGraph.h"
#include "linear_program_construction.h"
#include "utils.h"
#include <iostream>
#include <sstream>

namespace VariableConstruction {
    
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

    std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> generatePartitionCombinations(
        const std::unordered_map<int, std::vector<int>>& outgoingEdges,
        const std::unordered_map<int, std::vector<int>>& incomingEdges) {
        
        // Vector to store all the partition combinations for each vertex
        std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> allCombinations;

        // Vector of vertices to iterate over
        std::vector<int> vertices;
        for (const auto& [vertex, _] : outgoingEdges) {
            vertices.push_back(vertex);
        }

        // Helper function to generate combinations recursively
        std::function<void(int, std::unordered_map<int, std::vector<std::vector<int>>>&)> generate =
            [&](int index, std::unordered_map<int, std::vector<std::vector<int>>>& currentCombination) {
                if (index == vertices.size()) {
                    // Base case: if we've processed all vertices, add the current combination to the list
                    allCombinations.push_back(currentCombination);
                    return;
                }

                int vertex = vertices[index];
                //make sure no position 0 exists
                std::vector<int> edges_at_vertex = outgoingEdges.at(vertex);
                std::vector<int> incoming = incomingEdges.at(vertex);
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
        for (const auto& vertexEdges: originalGraph.getEdges()){
            for (const std::pair<int,int> incoming_vertex_and_position: vertexEdges.second){
                outgoing_vertex_to_edge_positions[vertexEdges.first].push_back(incoming_vertex_and_position.second);
                incoming_vertex_to_edge_positions[incoming_vertex_and_position.first].push_back(incoming_vertex_and_position.second);
            }
        }


        // Step 2: Iterate over all combinations of partitions (Cartesian product) - vector of maps from vertex to its partition given in edge positions.
        std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> partitionCombination = generatePartitionCombinations(outgoing_vertex_to_edge_positions, incoming_vertex_to_edge_positions);
        // int i=0;
        // for (const auto& partitionChoice : partitionCombination) {
        //     std::cout<<"partition number "<<i<<"\n";
        //     int j=0;
        //     for (const auto& vertexPartition : partitionChoice) {
        //         std::cout<<"vertex: "<<vertexPartition.first<<", j="<<j<<"\n";
        //         const auto& subsets = vertexPartition.second;
        //         int k=0;
        //         for (const auto& subset : subsets) {
        //             std::cout<<"subset "<<k<<" positions: \n";
        //             for (const int position: subset){
        //                 std::cout<<position<<", ";
        //             }
        //             std::cout<<"\n";
        //             k++;
        //         }
        //         j++;
        //     }
        //     i++;
        // }

        // Step 3: Construct new graphs based on each partition combination
        for (const auto& partitionChoice : partitionCombination) {
            WhiteheadGraph newGraph(0, true);
            std::unordered_map<int, int> position_to_new_source;
            std::unordered_map<int, int> position_to_new_target;

            // Add new vertices and track original vertices
            int newVertexId = 0;
            for (const auto& vertexPartition : partitionChoice) {
                int originalVertex = vertexPartition.first;
                const auto& subsets = vertexPartition.second;
                for (const auto& subset : subsets) {
                    newGraph.addVertex(newVertexId);
                    newGraph.rememberOriginalVertex(originalVertex, newVertexId);
                    for (const int position: subset){
                        assert (position!=0 && "bug - somehow position=0") ;
                        if (position > 0){
                            position_to_new_source[position] = newVertexId;
                        } else{
                            position_to_new_target[-position] = newVertexId;
                        }
                    }
                    newVertexId++;
                }
            }
            // Add edges to the new graph
            for (const auto& position_and_source : position_to_new_source) {
                newGraph.addEdge(position_and_source.second, position_to_new_target.at(position_and_source.first), position_and_source.first);
            }

            newGraphs.push_back(newGraph);
        }

        return newGraphs;
    }
}