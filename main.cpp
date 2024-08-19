// main.cpp
#include "CoreWhiteheadGraph.h"
#include <vector>

int main() {
    std::vector<int> word = {1, 2, -3, 1}; // Example of a cyclically reduced word
    int r = 3;

    CoreWhiteheadGraph graph = constructCoreWhiteheadGraph(word, r);
    graph.displayGraph();

    return 0;
}
