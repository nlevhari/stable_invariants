#ifndef WHITEHEAD_GRAPH_H
#define WHITEHEAD_GRAPH_H

#include "Graph.h"
#include <unordered_map>

class WhiteheadGraph : public Graph {
public:
    WhiteheadGraph(int r);
    WhiteheadGraph(int s, bool not_core);
    
    
    bool isValid(const bool partitioned=false) const;
    // WhiteheadGraph(const CoreWhiteheadGraph& g);
    std::vector<WhiteheadGraph> getConnectedComponents() const;
    int getR() const {return r;}

protected:
    int r;
};

#endif // WHITEHEAD_GRAPH_H
