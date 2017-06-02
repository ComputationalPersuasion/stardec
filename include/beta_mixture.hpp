#ifndef STARDEC_BETA_MIXTURE_H
#define STARDEC_BETA_MIXTURE_H

#include <vector>
#include <utility>
#include <numeric>
#include <boost/math/distributions/beta.hpp>

namespace stardec {
    class beta_mixture {
    public:
        beta_mixture() { _mixture.push_back(std::make_pair(1.0, boost::math::beta_distribution<>(1,1))); }
        beta_mixture(const std::vector<std::pair<double, boost::math::beta_distribution<>>> &mixture) : _mixture(mixture) {}

        const std::vector<std::pair<double, boost::math::beta_distribution<>>>& mixture() const {return _mixture;}

        double probability_belief_above(double belief) {
            return std::accumulate(_mixture.cbegin(), _mixture.cend(), 0.0, [belief](double acc, auto p) {return acc + p.first * cdf(complement(p.second, belief));});
        }
    private:
        std::vector<std::pair<double, boost::math::beta_distribution<>>> _mixture;
    };
}

#endif //STARDEC_BETA_MIXTURE_H
