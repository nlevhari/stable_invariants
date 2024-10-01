#include "linear_program_construction.h"

const std::vector<double> EquationConstruction::getObjective(const std::vector<WhiteheadGraph> & graphs){
    std::vector<double> minus_chi_per_graph;
    minus_chi_per_graph.reserve(graphs.size());
    for (int i=0; i<graphs.size(); i++){
        const auto& graph = graphs[i];
        minus_chi_per_graph.push_back(graph.getVertices().size() / 2 - graph.getConnectedComponents().size());
    }
    return minus_chi_per_graph;
}