#include <iostream>
#include <vector>
#include <numeric>  // For std::gcd
#include <cmath>    // For std::round
#include <sstream>
#include <memory>
#include "CoreWhiteheadGraph.h"
#include "WhiteheadGraph.h"
#include "linear_program_construction.h"

enum class PROBLEM_TYPE{
    SPI = 0,
    SPI_M = 1,
    SPI_NO_ORIGAMI = 2
};

// Helper function to parse command line input into a vector of integers
std::vector<int> parseWordInput(int argc, char* argv[], const PROBLEM_TYPE problem_type) {
    std::vector<int> word;
    int start = 3;
    if (problem_type == PROBLEM_TYPE::SPI_M){
        start++;
    }
    for (int i = start; i < argc; ++i) {
        word.push_back(std::stoi(argv[i]));
    }
    return word;
}

// Helper function to display the WhiteheadGraph objects based on their indices
void displayWhiteheadGraphs(const std::vector<WhiteheadGraph>& graphs, const std::vector<int>& indices) {
    for (int idx : indices) {
        std::cout << "Graph at index " << idx << ":\n";
        graphs[idx].printAdjacencyMatrix();
        graphs[idx].displayGraph();
    }
}

void displayWhiteheadGraphs(const std::vector<WhiteheadGraph>& graphs) {
    for (size_t idx=0; idx < graphs.size(); idx++) {
        std::cout << "Graph at index " << (int)idx << ":\n";
        graphs[idx].printAdjacencyMatrix();
        graphs[idx].displayGraph();
    }
}

int gcd(int a, int b) {
    return std::gcd(a, b);
}

// Function to calculate LCM (Least Common Multiple)
int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

// Function to convert a double to a fraction
std::pair<int, int> doubleToFraction(double num) {
    const double epsilon = 1e-9;  // Tolerance for floating-point comparisons
    int denominator = 1;
    
    while (std::fabs(num - std::round(num)) > epsilon) {
        num *= 10;
        denominator *= 10;
    }

    return {static_cast<int>(num), denominator};
}

// Function to find the LCM of denominators in a vector of doubles
int findLCMOfDenominators(const std::vector<double>& numbers) {
    std::vector<int> denominators;
    
    // Extract the denominators
    for (double num : numbers) {
        auto fraction = doubleToFraction(num);
        denominators.push_back(fraction.second);  // Push back the denominator
    }
    
    // Compute the LCM of all denominators
    int lcmValue = denominators[0];
    for (size_t i = 1; i < denominators.size(); ++i) {
        lcmValue = lcm(lcmValue, denominators[i]);
    }
    
    return lcmValue;
}

std::vector<int> convert_double_vec_to_int_vec(const std::vector<double>& vec) {
    const int lcm = findLCMOfDenominators(vec);
    std::vector<int> int_vec;
    int_vec.reserve(vec.size());
    for (const double d: vec){
        int_vec.push_back(static_cast<int>(d * lcm + 0.5));
    }
    return int_vec;
}

std::vector<int> getNonZeroIndices(const std::vector<double>& vec) {
    std::vector<int> indices;
    const double epsilon = 1e-9;  // Tolerance for floating-point comparisons
    for (size_t i = 0; i < vec.size(); ++i) {
        if (abs(vec[i]) > epsilon) {
            indices.push_back(i);  // Store the index of non-zero value
        }
    }
    return indices;
}

void build_linear_program(std::vector<WhiteheadGraph>& filteredGraphs, std::vector<WhiteheadGraph>& partitioned_graphs, std::vector<std::pair<std::vector<int>, std::vector<int>>>& constraint_pairs, const std::vector<int>& word, PROBLEM_TYPE problem_type, int m=0) {
    std::unique_ptr<StableInvariant> invariant;
    switch (problem_type){
        case PROBLEM_TYPE::SPI:
            return; // currently not supported
        case PROBLEM_TYPE::SPI_M:
            invariant = std::make_unique<Spi_m>(m); break;
        case PROBLEM_TYPE::SPI_NO_ORIGAMI:
            invariant = std::make_unique<SpiNoOrigami>(); break;
        default:
            return; // currently not supported
    };
    if (!invariant) return;
    // Step 4: Filter the list of partitioned subgraphs
    filteredGraphs = VariableConstruction::filterValidWhiteheadGraphsFromPartitions(partitioned_graphs, *invariant);
    // displayWhiteheadGraphs(filteredGraphs);
    std::cout<<"after filtered, got "<<filteredGraphs.size()<<" subgraphs:\n";
    // displayWhiteheadGraphs(filteredGraphs);

    // Step 5: Get the gluing constraints
    constraint_pairs = EquationConstruction::getGluingEquations(filteredGraphs, word.size(), *invariant);

    return;

}


int main(int argc, char* argv[]) {
    if (argc < 3 || (PROBLEM_TYPE(std::stoi(argv[1])) == PROBLEM_TYPE::SPI_M && argc < 4)) {
        std::cerr << "Usage: " << argv[0] << " problem_type r {m(if spi_m)} word_as_integers " << std::endl;
        return 1;
    }

    const PROBLEM_TYPE problem_type = PROBLEM_TYPE(std::stoi(argv[1]));

    // Parse the rank of the free group
    int r = std::stoi(argv[2]);

    // Parse the word from command line arguments
    std::vector<int> word = parseWordInput(argc, argv, problem_type);

    // Step 1: Create the CoreWhiteheadGraph from the word
    CoreWhiteheadGraph coreWHGraph = constructCoreWhiteheadGraph(word, r);

    // Step 2: Generate valid subgraphs for any problem
    StableInvariant invariant = StableInvariant();
    std::vector<WhiteheadGraph> validSubgraphs = VariableConstruction::findValidSubgraphsFromCore(coreWHGraph, invariant);
    std::cout<<"after valid, got "<<validSubgraphs.size()<<" subgraphs:\n";
    // Step 3: Create a new list of subgraphs by partitioning and replacing vertices
    std::vector<WhiteheadGraph> partitionedGraphs;
    if (problem_type == PROBLEM_TYPE::SPI_M) { 
        partitionedGraphs = validSubgraphs; 
    } else {
        for (const auto& subgraph : validSubgraphs) {
            std::vector<WhiteheadGraph> partitioned = VariableConstruction::partitionAndReplaceVertices(subgraph);
            partitionedGraphs.insert(partitionedGraphs.end(), partitioned.begin(), partitioned.end());
        }
    }
    std::cout<<"after partitioned, got "<<partitionedGraphs.size()<<" subgraphs:\n";

    std::vector<std::pair<std::vector<int>, std::vector<int>>> constraint_pairs;

    std::vector<WhiteheadGraph> filteredGraphs;

    const int m = problem_type == PROBLEM_TYPE::SPI_M ? std::stoi(argv[3]): -1;

    build_linear_program(filteredGraphs, partitionedGraphs, constraint_pairs, word, problem_type, m);

    // Step 6: Get the objective function from the graphs
    const auto& obj_coeffs = EquationConstruction::getObjective(filteredGraphs);

    // Step 7: Find graphs that contain an edge from the first letter of the original word
    std::vector<int> indicesWithFirstLetterEdge = EquationConstruction::findGraphsWithFirstLetterEdge(filteredGraphs);
    
    // Step 8: Solve the linear program
    if (!filteredGraphs.size()) { std::cout<<"no filtered graphs, terminating"<<std::endl; return 0;}
    const auto& obj_and_sol = getLinearProgramSolutionAndMinimizer(obj_coeffs, constraint_pairs, indicesWithFirstLetterEdge);
    displayWhiteheadGraphs(filteredGraphs, getNonZeroIndices(obj_and_sol.second));

    return 0;
}
