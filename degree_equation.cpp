#include "WhiteheadGraph.h"
#include "linear_program_construction.h"
#include <vector>

namespace EquationConstruction{

    std::vector<int> findGraphsWithFirstLetterEdge(const std::vector<WhiteheadGraph>& graphs) {
        std::vector<int> result;

        // Iterate over the list of WhiteheadGraphs
        for (size_t i = 0; i < graphs.size(); ++i) {
            const auto& graph = graphs[i];

            // Iterate over all edges in the current WhiteheadGraph
            bool found_graph = false;
            for (const auto& edgeList : graph.getEdges()) {
                for (const auto& edge : edgeList.second) {
                    if (edge.second == 1) { // Check if the edge originated from the first letter (position index 0)
                        result.push_back(static_cast<int>(i));
                        found_graph = true;
                        break; // Stop searching further in this graph
                    }
                }
                if (found_graph) break;
            }
        }

        return result;
    }
}
