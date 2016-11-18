#ifndef STARDEC_AGGREGATION_FUNCTIONS_H
#define STARDEC_AGGREGATION_FUNCTIONS_H

namespace stardec {
    double average(const std::unordered_map<unsigned int, double> &values, const std::unordered_map<unsigned int, double> &beliefs) {
        double sum = std::accumulate(values.cbegin(), values.cend(), 0.0, [&beliefs](double s, auto v){return s + v.second * beliefs.at(v.first);});
        return sum / values.size();
    }
}

#endif //STARDEC_AGGREGATION_FUNCTIONS_H
