#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "WhiteheadGraph.h"
#include "stable_invariants_types.h"
#include <iostream>

namespace EquationConstruction{
    // Custom hash function for set of sets
    struct SetOfSetsHash {
        std::size_t operator()(const std::set<std::set<int>>& s) const {
            std::size_t seed = 0;
            for (const auto& inner_set : s) {
                for (int i : inner_set) {
                    seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
            }
            return seed;
        }
    };

    std::unordered_map<std::set<std::set<int>>, std::vector<int>, SetOfSetsHash> calculateGluingRestrictions(const std::vector<WhiteheadGraph>& graphs) {
        std::unordered_map<std::set<std::set<int>>, std::vector<int>, SetOfSetsHash> gluing_restrictions;

        for (size_t i = 0; i < graphs.size(); ++i) {
            const auto& graph = graphs[i];
            const auto& original_vertices = graph.getOriginalVertices();
            const auto& edges_by_position = graph.getEdgesByPosition();

            std::unordered_map<int, std::set<int>> original_to_positions;

            // Group positions by original vertex
            for (const auto& [new_vertex, original_vertex] : original_vertices) {
                for (const auto& [position, edge] : edges_by_position) {
                    if (edge.first == new_vertex) {
                        original_to_positions[original_vertex].insert(position);
                    } else if (edge.second == new_vertex) {
                        original_to_positions[original_vertex].insert(-position);
                    }
                }
            }

            // Create partition of positions for each original vertex
            for (const auto& [original_vertex, positions] : original_to_positions) {
                std::set<std::set<int>> partition_of_positions;
                partition_of_positions.insert(positions);

                auto it = gluing_restrictions.find(partition_of_positions);
                if (it == gluing_restrictions.end()) {
                    gluing_restrictions[partition_of_positions] = {static_cast<int>(i)};
                } else {
                    it->second.push_back(static_cast<int>(i));
                }
            }
        }

        return gluing_restrictions;
    }
    const std::set<std::set<int>> getOppositePartition(const std::set<std::set<int>>& partition, const int n){
        std::set<std::set<int>> opposite_partition;
        for (const auto& s : partition) {
            std::set<int> opposite_set;
            for (int position : s) {
                if (position > 1) {
                    opposite_set.insert(-(position - 1));
                } else if (position == 1) {
                    opposite_set.insert(-n);
                } else if (position <= -1 && position > -n) {
                    opposite_set.insert(-position + 1);
                } else { // position == -n
                    opposite_set.insert(1);
                }
            }
            opposite_partition.insert(opposite_set);
        }
        return opposite_partition;
    }

    std::vector<std::pair<std::vector<int>, std::vector<int>>> processGluingRestrictions(const std::unordered_map<std::set<std::set<int>>, std::vector<int>, SetOfSetsHash>& gluingRestrictions, int n) {
        std::unordered_set<std::set<std::set<int>>, SetOfSetsHash> visited;
        std::vector<std::pair<std::vector<int>, std::vector<int>>> output; // pairs of inverse half-edges.

        for (const auto& [key, value] : gluingRestrictions) {
            const auto& opposite_partition = getOppositePartition(key, n);

            std::vector<int> opposite_value;
            auto it = gluingRestrictions.find(opposite_partition);
            if (it != gluingRestrictions.end()) {
                opposite_value = it->second;
            }

            std::vector<int> value_vec(value.begin(), value.end());

            if (visited.find(key) == visited.end() && visited.find(opposite_partition) == visited.end()) {
                output.emplace_back(value_vec, opposite_value);
                visited.insert(key);
                visited.insert(opposite_partition);
            }
        }
        return output;
    }

    std::vector<std::pair<std::vector<int>, std::vector<int>>> getGluingEquations(const std::vector<WhiteheadGraph> & graphs, int n, StableInvariant& invariant){
        const auto& constraint_map = calculateGluingRestrictions(graphs);
        const auto& constraint_pairs = processGluingRestrictions(constraint_map, n);
        return constraint_pairs;
    }
}