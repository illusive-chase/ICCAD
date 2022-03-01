#ifndef IC_GRAPH_HH_
#define IC_GRAPH_HH_

#include <iostream>
#include <cstdint>
#include <string>
#include <optional>
#include <vector>

namespace ic {

    enum ComponentType {
        UNDEFINED,
        R,
        IS,
        VS,
        VCCS,
        VCVS,
        CCCS,
        CCVS,
    };

    struct Component {
        ComponentType key;
        uint32_t id;
        uint32_t np, nm;
        uint32_t ncp, ncm;
        double value;

        static std::optional<Component> get_from_line(std::istream& s_in);
    };

    struct Graph {

        struct Edge {
            std::vector<size_t> component_ids;
            size_t linked_node_id;
        };

        std::vector<Component> components;
        std::vector<std::list<Edge>> edge_table;

        Graph() {}

        bool empty() const;
        bool read_from_netlist(std::istream& s_in, bool quiet=false);
    }

}


#endif