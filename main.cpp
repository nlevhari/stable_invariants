#include <iostream>
#include <vector>
#include <sstream>
#include "CoreWhiteheadGraph.h"
#include "WhiteheadGraph.h"
#include "linear_program_construction.h"

// Helper function to parse command line input into a vector of integers
std::vector<int> parseWordInput(int argc, char* argv[]) {
    std::vector<int> word;
    for (int i = 2; i < argc; ++i) {
        word.push_back(std::stoi(argv[i]));
    }
    return word;
}

// Helper function to display the WhiteheadGraph objects based on their indices
void displayWhiteheadGraphs(const std::vector<WhiteheadGraph>& graphs, const std::vector<int>& indices) {
    for (int idx : indices) {
        std::cout << "Graph at index " << idx << ":\n";
        graphs[idx].displayGraph();
    }
}

void displayWhiteheadGraphs(const std::vector<WhiteheadGraph>& graphs) {
    for (size_t idx=0; idx < graphs.size(); idx++) {
        std::cout << "Graph at index " << (int)idx << ":\n";
        graphs[idx].displayGraph();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " r word_as_integers" << std::endl;
        return 1;
    }

    // Parse the rank of the free group
    int r = std::stoi(argv[1]);

    // Parse the word from command line arguments
    std::vector<int> word = parseWordInput(argc, argv);

    // Step 1: Create the CoreWhiteheadGraph from the word
    CoreWhiteheadGraph coreWHGraph = constructCoreWhiteheadGraph(word, r);

    // Step 2: Generate subgraphs with minimum degree of at least 2
    std::vector<WhiteheadGraph> validSubgraphs = VariableConstruction::findValidSubgraphsFromCore(coreWHGraph);
    std::cout<<"after valid, got "<<validSubgraphs.size()<<" subgraphs:\n";
    // Step 3: Create a new list of subgraphs by partitioning and replacing vertices
    std::vector<WhiteheadGraph> partitionedGraphs;
    for (const auto& subgraph : validSubgraphs) {
        std::vector<WhiteheadGraph> partitioned = VariableConstruction::partitionAndReplaceVertices(subgraph);
        partitionedGraphs.insert(partitionedGraphs.end(), partitioned.begin(), partitioned.end());
    }
    std::cout<<"after partitioned, got "<<partitionedGraphs.size()<<" subgraphs:\n";

    // Step 4: Filter the list of partitioned subgraphs
    std::vector<WhiteheadGraph> filteredGraphs = VariableConstruction::filterValidWhiteheadGraphsFromPartitions(partitionedGraphs);
    // displayWhiteheadGraphs(filteredGraphs);
    std::cout<<"after filtered, got "<<filteredGraphs.size()<<" subgraphs:\n";
    // displayWhiteheadGraphs(filteredGraphs);

    // Step 5: Find graphs that contain an edge from the first letter of the original word
    // std::vector<int> indicesWithFirstLetterEdge = EquationConstruction::findGraphsWithFirstLetterEdge(filteredGraphs);

    // Step 6: Display the filtered WhiteheadGraph objects
    // displayWhiteheadGraphs(filteredGraphs, indicesWithFirstLetterEdge);

    return 0;
}
