#ifndef stable_invariants_types_h
#define stable_invariants_types_h
#include <cassert>
#include "Graph.h"

class StableInvariant {
public:
    virtual ~StableInvariant() = default;

    virtual bool isValidWHGraphForInvariant(const Graph& g) const {
        return isValidWHGraph(g);
    }

protected:
    virtual bool isValidWHGraph(const Graph& g) const {
        const bool enough_edges = g.getEdges().size() >= 2;
        if (!enough_edges) return false;
        const bool at_least_deg_2 = g.hasMinimumDegree(2);
        if (!at_least_deg_2) return false;
        const bool connected = g.isConnected();
        return connected;
    }
};

class SpiNoOrigami : public StableInvariant {
protected:
    bool isValidWHGraph(const Graph& g) const override {
        return StableInvariant::isValidWHGraph(g) && g.isBiconnected(); 
    }
};

class Spi_m : public StableInvariant {
public:
    Spi_m(const int m_) : _m(m_) {};

protected:
    bool isValidWHGraph(const Graph& g) const override {
        assert(_m >= 0 && "m is not allowed to be <0");
        const bool normal_valid = StableInvariant::isValidWHGraph(g);
        if (!normal_valid) { return false; }
        const auto& vertices = g.getVertices();
        for (const auto& vertex : vertices) {
            const auto& incoming_outgoing = g.getNeighborVecsAtVertex(vertex);
            if ((static_cast<int>(incoming_outgoing.first.size()) - static_cast<int>(incoming_outgoing.second.size())) % _m != 0) {
                return false;
            }
        }
        return true;
    }

private:
    const int _m;
};

#endif