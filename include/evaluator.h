#ifndef STARDEC_EVALUATOR_H
#define STARDEC_EVALUATOR_H

#include <functional>
#include <random>
#include <utility>
#include <string>
#include "tree.h"
#include "node.h"

namespace stardec {
    typedef std::function<leafnode*(const leafnode* const)> evaluationfunction;

    class evaluator {
    public:
        evaluator();
        std::pair<std::vector<std::string>, std::vector<double>> evaluate(const leafnode* const root, const evaluationfunction &prop, const evaluationfunction &opp) const;
        double evaluate_mt(const leafnode* const root, const evaluationfunction &prop, const evaluationfunction &opp, unsigned int runs) const;

    private:
        std::uniform_int_distribution<> _u_i_d;
        std::random_device _rd;
    };
}

#endif
