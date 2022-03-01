#include "IC_Graph.hh"
#include "IO.hh"

namespace ic {
    
    std::optional<Component> Component::get_from_line(std::istream& s_in) {
        std::string line;
        if (!std::getline(s_in, line)) return std::nullopt;
        Component comp = {};
        char key = '\0';
        auto extra_node = [&key]() {
            return key != 'R' && key != 'I' && key != 'V';
        };
        if (!io::parse_from_string(
            line,
            io::concat(key, comp.id),
            comp.np,
            comp.nm,
            io::option(comp.ncp, extra_node),
            io::option(comp.ncm, extra_node),
            io::measure(comp.value)
        ))
            return std::nullopt;
        switch (key) {
            case 'R': comp.key = ComponentType::R; break;
            case 'I': comp.key = ComponentType::IS; break;
            case 'V': comp.key = ComponentType::VS; break;
            case 'G': comp.key = ComponentType::VCCS; break;
            case 'E': comp.key = ComponentType::VCVS; break;
            case 'H': comp.key = ComponentType::CCCS; break;
            case 'F': comp.key = ComponentType::CCVS; break;
        }
        if (comp.key == ComponentType::UNDEFINED) return std::nullopt;

        return comp;
    }

}