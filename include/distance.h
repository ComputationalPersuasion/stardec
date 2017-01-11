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

    double euclidean(const std::vector<double> &point, const std::vector<double> &ref, const std::vector<double> &min, const std::vector<double> &max) {
        double sum = 0.0;
        for(unsigned int i = 0; i < point.size(); i++)
            sum += std::pow((point[i] - ref[i]) / (max[i] - min[i]), 2);
        return std::sqrt(sum);
    }

    double weighted_euclidean(const std::vector<double> &point, const std::vector<double> &ref, const std::vector<double> &min,
                              const std::vector<double> &max, const std::vector<double> &w) {
        double sum = 0.0;
        for(unsigned int i = 0; i < point.size(); i++)
            sum += w[i] * std::pow((point[i] - ref[i]) / (max[i] - min[i]), 2);
        return std::sqrt(sum);
    }

    std::vector<double> idealpoint(const std::set<std::vector<double>> &points) {
        std::vector<double> ideal(points.cbegin()->size(), 0.0);
        for(auto point : points)
            std::transform(ideal.cbegin(), ideal.cend(), point.cbegin(), ideal.begin(), [](auto i, auto p){return std::max(i,p);});
        return ideal;
    }

    std::vector<double> biaised_ideal(const std::set<std::vector<double>> &points, const std::vector<double> &weights) {
        auto ideal = idealpoint(points);
        std::transform(ideal.cbegin(), ideal.cend(), weights.cbegin(), ideal.begin(), [](auto i, auto w){return i * w;});
        return ideal;
    }
}

#endif
