#ifndef STARDEC_AGGREGATION_FUNCTIONS_H
#define STARDEC_AGGREGATION_FUNCTIONS_H

namespace stardec {
    double average(const std::unordered_map<unsigned int, double> &values, const std::unordered_map<unsigned int, double> &beliefs) {
        return std::accumulate(values.cbegin(), values.cend(), 0.0, [&beliefs](auto acc, auto v) {return acc + (v.second + beliefs.at(v.first)) / 2;}) / values.size();
    }

    std::vector<double> mc(const std::unordered_map<unsigned int, std::vector<double>> &values, const std::unordered_map<unsigned int, double> &beliefs) {
        std::vector<double> vals;
        for(auto g : values) {
            for(auto v : g.second)
                vals.push_back(v);
            vals.push_back(beliefs.at(g.first));
        }
        return vals;
    }
}

#endif //STARDEC_AGGREGATION_FUNCTIONS_H
