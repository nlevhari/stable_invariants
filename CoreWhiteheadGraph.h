// CoreWhiteheadGraph.h
#ifndef COREWHITEHEADGRAPH_H
#define COREWHITEHEADGRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <utility>
#include "Graph.h"
#include "WhiteheadGraph.h"

class CoreWhiteheadGraph : public WhiteheadGraph {
public:
    CoreWhiteheadGraph(int r);
};

CoreWhiteheadGraph constructCoreWhiteheadGraph(const std::vector<int>& word, int r);

#endif // COREWHITEHEADGRAPH_H
