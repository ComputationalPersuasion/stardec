#ifndef STARDEC_VALUATION_FUNCTIONS_H
#define STARDEC_VALUATION_FUNCTIONS_H

#include <unordered_map>
#include <vector>
#include "graph.h"

namespace stardec {
    //All goals, not just the positive ones => do not use with logical expression containing NOTs.
    std::unordered_map<unsigned int, double> present(const graph &g, const std::vector<std::string> &execution) {
        std::unordered_map<unsigned int, double> values;
        for(auto arg : g.goals())
            values[arg->id()] = (arg->is_acceptable(g, execution) ? 1.0 : 0.0);
        return values;
    }

    std::unordered_map<unsigned int, double> logicalvalid(const graph &g, const std::vector<std::string> &execution) {
        std::unordered_map<unsigned int, double> values;
        values[0] = g.logicalvalid(execution);
        return values;
    }
}

#endif //STARDEC_VALUATION_FUNCTIONS_H
