#ifndef STARDEC_EVALUATOR_H
#define STARDEC_EVALUATOR_H

#include <functional>
#include <random>
#include <utility>
#include <string>
#include "tree.h"
#include "node.h"

namespace stardec {
    template <class T>
    using evaluationfunction = typename std::function<leafnode<T>*(const leafnode<T>* const)>;

    template <class T>
    class evaluator {
    public:
        evaluator();
        std::pair<std::vector<std::string>, T> evaluate(const leafnode<T>* const root, const evaluationfunction<T> &prop, const evaluationfunction<T> &opp) const;
        double evaluate_mt(const leafnode<T>* const root, const evaluationfunction<T> &prop, const evaluationfunction<T> &opp, unsigned int runs) const;

    private:
        std::uniform_int_distribution<> _u_int_d;
        std::random_device _rd;
    };
}

#endif
