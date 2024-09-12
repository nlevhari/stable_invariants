#ifndef WHITEHEAD_GRAPH_H
#define WHITEHEAD_GRAPH_H

#include <vector>
#include "Graph.h"

class WhiteheadGraph : public Graph {
public:
    WhiteheadGraph(int r);
    WhiteheadGraph(int s, bool not_core);
    WhiteheadGraph(std::vector<int> vertices);
    WhiteheadGraph(Graph G_);
        
    bool isValid(const bool partitioned=false) const;
    // WhiteheadGraph(const CoreWhiteheadGraph& g);
    std::vector<WhiteheadGraph> getConnectedComponents() const;

};

#endif // WHITEHEAD_GRAPH_H
