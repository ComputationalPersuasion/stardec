#ifndef STARDEC_UPDATE_FUNCTIONS_H
#define STARDEC_UPDATE_FUNCTIONS_H

#include <algorithm>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "argument.hpp"
#include "graph.hpp"
#include "values.hpp"
#include "function_alias.hpp"

namespace stardec {
    template <typename T, typename... Values>
    class update_function {
    public:
        virtual T update(argument<Values...> *arg) const;

    private:
        bool _final;
    };


    /***************** belief_update ********************/
    template <typename... Values>
    class belief_update : public update_function<belief, Values...> {
    public:
        belief_update(belief_function<Values...> bel) : update_function<belief, Values...>(), _fct(bel) {}

        belief update(argument<Values...> *arg) const override {
            return _fct(arg);
        }

    private:
        belief_function<Values...> _fct;
    };

    template <typename... Values>
    void fast_refine(argument<Values...> *arg, bool side, double factor) {
        double old_belief = arg->template get<belief>().value();
        if(side)
            arg->template get<belief>().value() = old_belief + factor * (1 - old_belief);
        else
            arg->template get<belief>().value() = old_belief * (1 - factor);
    }

    template <typename... Values>
    bool attackers_below_half_belief(const argument<Values...> * const arg) {
        auto atkers = arg->get_attackers();
        return std::all_of(atkers.cbegin(), atkers.cend(), [](auto atk){return atk.second->template get<belief>().value() <= 0.5;});
    }

    template <typename... Values>
    belief fast_general_update(argument<Values...> *arg, double factor) {
        if(attackers_below_half_belief(arg)) {
            fast_refine(arg, true, factor);
            for(auto rel : arg->get_attacked())
                fast_refine(rel.second, false, factor);
        }
        return arg->template get<belief>();
    }

    template <typename... Values>
    belief fast_ambivalent(argument<Values...> *arg) {
        return fast_general_update(arg, 0.75);
    }

    template <typename... Values>
    belief fast_strict(argument<Values...> *arg) {
        return fast_general_update(arg, 1.0);
    }
    /***************** belief_update ********************/

    template <typename... Values>
    class affective_norm_update : update_function<affective_norm, Values...> {
    public:
        affective_norm_update(const std::unordered_map<std::string, std::array<double, 3>> &norm) : update_function<affective_norm, Values...>(), _norm(norm) {}

        affective_norm update(argument<Values...> *arg) const override {
            std::array<double, 3> mean {0.0, 0.0, 0.0};
            for(auto w : arg->words()) {
                auto norm = _norm.at(w);
                for(unsigned int i = 0; i < 3; i++)
                    mean[i] += (norm[i] / arg->words().size());
            }
            return affective_norm(mean);
        }
    private:
        const std::unordered_map<std::string, std::array<double, 3>> &_norm;
    };
}

#endif //STARDEC_UPDATE_FUNCTIONS_H
