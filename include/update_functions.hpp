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
    template <typename T, typename... Value>
    class update_function {
    public:
        virtual T update(argument<Value...> *arg) const;
    };

    /***************** belief_update ********************/
    template <typename... Value>
    class belief_update : public update_function<belief, Value...> {
    public:
        belief_update(belief_function<Value...> bel) : update_function<belief, Value...>(), _fct(bel) {}

        belief update(argument<Value...> *arg) const override {
            return _fct(arg);
        }

    private:
        belief_function<Value...> _fct;
    };

    template <typename... Value>
    void fast_refine(argument<Value...> *arg, bool side, double factor) {
        double old_belief = arg->template get<belief>().value();
        if(side)
            arg->template get<belief>().value() = old_belief + factor * (1 - old_belief);
        else
            arg->template get<belief>().value() = old_belief * (1 - factor);
    }

    template <typename... Value>
    bool attackers_below_half_belief(const argument<Value...> * const arg) {
        auto atkers = arg->get_attackers();
        return std::all_of(atkers.cbegin(), atkers.cend(), [](auto atk){return atk.second->template get<belief>().value() <= 0.5;});
    }

    template <typename... Value>
    belief fast_general_update(argument<Value...> *arg, double factor) {
        if(attackers_below_half_belief(arg)) {
            fast_refine(arg, true, factor);
            for(auto rel : arg->get_attacked())
                fast_refine(rel.second, false, factor);
        }
        return arg->template get<belief>();
    }

    template <typename... Value>
    belief fast_ambivalent(argument<Value...> *arg) {
        return fast_general_update(arg, 0.75);
    }

    template <typename... Value>
    belief fast_strict(argument<Value...> *arg) {
        return fast_general_update(arg, 1.0);
    }
    /***************** belief_update ********************/

    /************** affective_norm_update ***************/
    template <typename... Value>
    class affective_norm_update : update_function<affective_norm, Value...> {
    public:
        affective_norm_update(const std::unordered_map<std::string, std::array<double, 3>> &norm) : update_function<affective_norm, Value...>(), _norm(norm) {}

        affective_norm update(argument<Value...> *arg) const override {
            std::array<double, 3> mean {0.0, 0.0, 0.0};
            for(auto w : arg->words()) {
                auto norm = _norm.at(w);
                for(unsigned int i = 0; i < 3; i++)
                    mean[i] += (norm[i] / arg->words().size());
            }
            return arg->template get<affective_norm>() = affective_norm(mean);
        }
    private:
        const std::unordered_map<std::string, std::array<double, 3>> &_norm;
    };
    /************** affective_norm_update ***************/
}

#endif //STARDEC_UPDATE_FUNCTIONS_H
