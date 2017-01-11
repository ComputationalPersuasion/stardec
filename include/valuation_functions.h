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

    std::unordered_map<unsigned int, std::vector<double>> circumplex(const graph &g, const std::vector<std::string> &execution,
                                                                     const std::unordered_map<std::string, std::array<double, 3>> &cir_values) {
        std::unordered_map<unsigned int, std::vector<double>> values;
        auto present_values = present(g, execution);
        auto mean_aff_norm = [&cir_values] (std::vector<double> acc, std::string word) {
            for(unsigned int i = 0; i < 3; i++)
                acc[i] += (cir_values.at(word)[i] / acc[3]);
            return acc;
        };

        std::array<double, 3> aff_norms = {10, 10, 10};
        for(auto arg_lbl : execution) {
            auto arg = g.arg(arg_lbl);
            auto mean_norm = std::accumulate(arg->words().cbegin(), arg->words().cend(), std::vector<double>({0.0, 0.0, 0.0, (double)arg->words().size()}), mean_aff_norm);
            for(unsigned int i = 0; i < 3; i++)
                aff_norms[i] = std::min(aff_norms[i], mean_norm[i]);
        }
        for(auto arg : g.goals())
            values[arg->id()] = {aff_norms[0], aff_norms[1], aff_norms[2], present_values[arg->id()]};

        return values;
    }

    std::unordered_map<unsigned int, double> logicalvalid(const graph &g, const std::vector<std::string> &execution) {
        std::unordered_map<unsigned int, double> values;
        values[0] = g.logicalvalid(execution);
        return values;
    }
}

#endif //STARDEC_VALUATION_FUNCTIONS_H
