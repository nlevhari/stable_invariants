#include "WhiteheadGraph.h"
#include "Graph.h"
#include <stack>
#include <cassert>
#include <iostream>
#include <sstream>

const std::vector<int> generateVerticesFromR(const int r){
    std::vector<int> vertices;
    for (int i = 0; i < r; ++i) {
        vertices.push_back(i + 1);
        vertices.push_back(-(i + 1));
    };
    return vertices;
}

WhiteheadGraph::WhiteheadGraph(int r) : Graph(generateVerticesFromR(r)) {}

WhiteheadGraph::WhiteheadGraph(Graph G_) : Graph(G_) {}

const std::vector<int> generate_s_vertices(const int s){
    std::vector<int> vertices;
    for (int i = 0; i < s; ++i) {
        vertices.push_back(i);
    };
    return vertices;
}

WhiteheadGraph::WhiteheadGraph(int s, bool not_core) : Graph(generate_s_vertices(s)) {
    assert(not_core && "bad instantiation for WHG"); // if not_core=false, bad instantiation.
}

WhiteheadGraph::WhiteheadGraph(const std::vector<int> vertices_) : Graph(vertices_) { }

void WhiteheadGraph::addEdge(int start, int end, int position){
    assert(position!=0 && "We dont allow position=0 for WH graphs");
    Graph::addEdge(start, end, position);
}


