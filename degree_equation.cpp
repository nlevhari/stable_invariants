#include "WhiteheadGraph.h"
#include "/Users/nivlevhari/repos/uni/stable_invariants/linear_program_construction.h"
#include <vector>

namespace EquationConstruction{

    std::vector<int> findGraphsWithFirstLetterEdge(const std::vector<WhiteheadGraph>& graphs) {
        std::vector<int> result;

        // Iterate over the list of WhiteheadGraphs
        for (size_t i = 0; i < graphs.size(); ++i) {
            const auto& graph = graphs[i];

            // Iterate over all edges in the current WhiteheadGraph
            for (const auto& edgeList : graph.getEdges()) {
                for (const auto& edge : edgeList.second) {
                    if (edge.second == 0) { // Check if the edge originated from the first letter (position index 0)
                        result.push_back(static_cast<int>(i));
                        break; // Stop searching further in this graph
                    }
                }
            }
        }

        return result;
    }
}
