#ifndef STARDEC_VALUATION_FUNCTIONS_H
#define STARDEC_VALUATION_FUNCTIONS_H

#include <unordered_map>
#include <vector>
#include "graph.h"

namespace stardec {
    //All goals, not just the positive ones => do not use with logical expression containing NOTs.
    std::unordered_map<unsigned int, std::vector<double>> present(const graph &g, const std::vector<std::string> &execution) {
        std::unordered_map<unsigned int, std::vector<double>> values;
        for(auto arg : g.goals())
            values[arg->id()].push_back((arg->is_acceptable(g, execution) ? 1.0 : 0.0));
        return values;
    }

    std::unordered_map<unsigned int, std::vector<double>> logicalvalid(const graph &g, const std::vector<std::string> &execution) {
        std::unordered_map<unsigned int, std::vector<double>> values;
        values[0].push_back(g.logicalvalid(execution));
        return values;
    }
}

#endif //STARDEC_VALUATION_FUNCTIONS_H
