#ifndef linear_program_construction_h
#define linear_program_construction_h

#include <vector>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <glpk.h>

#include "WhiteheadGraph.h"
#include "CoreWhiteheadGraph.h"
#include "linear_program_construction.h"
#include "stable_invariants_types.h"


namespace VariableConstruction {

    // Declaration of functions from generate_subgraphs.cpp
    std::vector<std::vector<Edge>> generateSubsets(const std::vector<Edge>& edges);

    WhiteheadGraph buildSubgraph(const CoreWhiteheadGraph& graph, const std::vector<Edge>& subset);

    std::vector<WhiteheadGraph> findValidSubgraphsFromCore(const CoreWhiteheadGraph& graph, StableInvariant& invariant);


    // Declaration of functions from unfolding.cpp

    std::vector<WhiteheadGraph> partitionAndReplaceVertices(const WhiteheadGraph& subgraph);

    std::vector<WhiteheadGraph> filterValidWhiteheadGraphsFromPartitions(const std::vector<WhiteheadGraph>& graphs, StableInvariant& invariant);

}

namespace EquationConstruction{

    // Declaration of functions from degree_equation.cpp
    std::vector<int> findGraphsWithFirstLetterEdge(const std::vector<WhiteheadGraph>& graphs);

    std::vector<std::pair<std::vector<int>, std::vector<int>>> getGluingEquations(const std::vector<WhiteheadGraph> & graphs, int n, StableInvariant& invariant);

    const std::vector<double> getObjective(const std::vector<WhiteheadGraph> & graphs);

}

std::pair<double, std::vector<double>> getLinearProgramSolutionAndMinimizer(std::vector<double> objective_coeffs, std::vector<std::pair<std::vector<int>, std::vector<int>>> constraint_pairs, std::vector<int> degree_equation);

#endif // linear_program_construction_h
