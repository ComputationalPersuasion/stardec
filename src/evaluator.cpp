#include <future>
#include <exception>
#include "evaluator.h"

namespace stardec {
    evaluator::evaluator() : _u_i_d(0,99) {}

    std::pair<std::vector<std::string>, std::vector<double>> evaluator::evaluate(const leafnode * const root, const evaluationfunction &prop, const evaluationfunction &opp) const {
        if(root->children.empty())
            throw std::invalid_argument("Empty decision tree.");
        std::vector<std::string> trace;
        const leafnode * current = root;
        while(!current->is_leaf()) {
            current = prop(current);
            trace.push_back(current->label);
            if(!current->is_leaf()) {
              current = opp(current);
              trace.push_back(current->label);
            } else
                break;
        }
        return std::make_pair<>(trace, current->values);
    }

    double evaluator::evaluate_mt(const leafnode* const root, const evaluationfunction &prop, const evaluationfunction &opp, unsigned int runs) const {
        if(root->children.empty())
            throw std::invalid_argument("Empty decision tree.");
        double sum = 0;
        std::vector<std::future<std::pair<std::vector<std::string>, std::vector<double>>>> futures;
        for(unsigned int i = 0; i < runs; i++)
            futures.push_back(std::async(std::launch::async, &evaluator::evaluate, this, std::cref(root), std::cref(prop), std::cref(opp)));
        for(auto &f : futures)
            sum += f.get().second[0] / runs;
        return sum;
    }
}
