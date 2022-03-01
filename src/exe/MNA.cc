#include "IC_Graph.hh"

int main() {
    while (1) {
        auto res = ic::Component::get_from_line(std::cin);
        if (!res) {
            std::cout << "Fail" << std::endl;
            continue;
        }
        auto comp = *res;
        std::cout << static_cast<int>(comp.key) << ' '
                  << comp.id << ' '
                  << comp.np << ' '
                  << comp.nm << ' '
                  << comp.ncp << ' '
                  << comp.ncm << ' '
                  << comp.value << std::endl;
    }
}