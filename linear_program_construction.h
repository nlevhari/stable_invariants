#ifndef linear_program_construction_h
#define linear_program_construction_h

#include <vector>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <algorithm>
#include "WhiteheadGraph.h"
#include "CoreWhiteheadGraph.h"
#include "linear_program_construction.h"

namespace VariableConstruction {

    // Declaration of functions from generate_subgraphs.cpp
    std::vector<std::vector<Edge>> generateSubsets(const std::vector<Edge>& edges);

    WhiteheadGraph buildSubgraph(const CoreWhiteheadGraph& graph, const std::vector<Edge>& subset);

    std::vector<WhiteheadGraph> findValidSubgraphsFromCore(const CoreWhiteheadGraph& graph);

    // Declaration of functions from unfolding.cpp
    std::vector<std::vector<std::vector<std::pair<int, int>>>> generatePartitions(const std::list<std::pair<int, int>>& edges);

    std::vector<WhiteheadGraph> partitionAndReplaceVertices(const WhiteheadGraph& subgraph);
    std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> generatePartitionCombinations(const std::unordered_map<int, std::vector<int>>& outgoingEdges, const std::unordered_map<int, std::vector<int>>& incomingEdges);

    std::vector<WhiteheadGraph> filterValidWhiteheadGraphsFromPartitions(const std::vector<WhiteheadGraph>& graphs);

}

namespace EquationConstruction{

    // Declaration of functions from degree_equation.cpp
    std::vector<int> findGraphsWithFirstLetterEdge(const std::vector<WhiteheadGraph>& graphs);

}

#endif // linear_program_construction_h
