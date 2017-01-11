#include <future>
#include <exception>
#include "evaluator.h"

namespace stardec {
    template <class T>
    evaluator<T>::evaluator() : _u_int_d(0,99) {}

    template <class T>
    std::pair<std::vector<std::string>, T> evaluator<T>::evaluate(const leafnode<T> * const root, const evaluationfunction<T> &prop, const evaluationfunction<T> &opp) const {
        if(root->children.empty())
            throw std::invalid_argument("Empty decision tree.");
        std::vector<std::string> trace;
        const leafnode<T>* current = root;
        while(!current->is_leaf()) {
            current = prop(current);
            trace.push_back(current->label);
            if(!current->is_leaf()) {
              current = opp(current);
              trace.push_back(current->label);
            } else
                break;
        }
        return std::make_pair<>(trace, current->value);
    }

    template <class T> double manage_val(const T &val);
    template<> double manage_val(const double &val) {
        return val;
    }
    template<> double manage_val(const std::vector<double> &val) {
        return std::accumulate(val.cbegin(), val.cend(), 0.0) / val.size();
    }

    template <class T>
    double evaluator<T>::evaluate_mt(const leafnode<T>* const root, const evaluationfunction<T> &prop, const evaluationfunction<T> &opp, unsigned int runs) const {
        if(root->children.empty())
            throw std::invalid_argument("Empty decision tree.");
        double sum = 0;
        std::vector<std::future<std::pair<std::vector<std::string>, T>>> futures;
        for(unsigned int i = 0; i < runs; i++)
            futures.push_back(std::async(std::launch::async, &evaluator::evaluate, this, std::cref(root), std::cref(prop), std::cref(opp)));
        for(auto &f : futures)
            sum += manage_val(f.get().second) / runs;
        return sum;
    }

    template class evaluator<double>;
    template class evaluator<std::vector<double>>;
}
