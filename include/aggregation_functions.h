#ifndef STARDEC_AGGREGATION_FUNCTIONS_H
#define STARDEC_AGGREGATION_FUNCTIONS_H

namespace stardec {
    double average(const std::unordered_map<unsigned int, double> &values, const std::unordered_map<unsigned int, double> &beliefs) {
        double sum = 0.0;
        int cpt = 0;
        for(auto p : values) {
            cpt++;
            sum += p.second * beliefs.at(p.first);
        }
        return sum / cpt;
    }
}

#endif //STARDEC_AGGREGATION_FUNCTIONS_H
