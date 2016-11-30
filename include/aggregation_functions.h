#ifndef STARDEC_AGGREGATION_FUNCTIONS_H
#define STARDEC_AGGREGATION_FUNCTIONS_H

namespace stardec {
    std::vector<double> average(const std::unordered_map<unsigned int, std::vector<double>> &values, const std::unordered_map<unsigned int, double> &beliefs) {
        double sum = 0.0;
        for(auto v : values)
            sum += (std::accumulate(v.second.cbegin(), v.second.cend(), 0.0) + beliefs.at(v.first)) / (v.second.size() + 1);
        return std::vector<double>({sum / values.size()});
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
