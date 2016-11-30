#ifndef STARDEC_DISTANCE_H
#define STARDEC_DISTANCE_H

#include <vector>
#include <cmath>

namespace stardec {
    double kullback_leibler(const std::vector<double> &model, const std::vector<double> &profile) {
        double sum = 0;
        for(unsigned int i = 0; i < model.size(); i++)
            sum += model[i] * std::log2(model[i] / profile[i]);
        return sum;
    }

    double jensen_shannon(const std::vector<double> &model, const std::vector<double> &profile) {
        std::vector<double> intermediate(model.size());
        std::transform(model.cbegin(), model.cend(), profile.cbegin(), intermediate.begin(), [](auto m, auto p){return 1.0/2*(m+p);});
        return 1.0 / 2 * kullback_leibler(model, intermediate) + 1.0 / 2 * kullback_leibler(profile, intermediate);
    }
}

#endif
