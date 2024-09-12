// CoreWhiteheadGraph.cpp
#include "CoreWhiteheadGraph.h"
#include <iostream>

CoreWhiteheadGraph::CoreWhiteheadGraph(int r_) : WhiteheadGraph(r_), r(r_) { }

CoreWhiteheadGraph constructCoreWhiteheadGraph(const std::vector<int>& word, int r) {
    CoreWhiteheadGraph graph(r);

    int n = word.size();
    for (int i = 0; i < n; ++i) {
        int k = word[i];
        int l = word[(i + 1) % n];

        int start = -k;
        int end = l;

        graph.addEdge(start, end, i+1);
    }

    return graph;
}
