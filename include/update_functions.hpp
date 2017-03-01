#ifndef STARDEC_UPDATE_FUNCTIONS_H
#define STARDEC_UPDATE_FUNCTIONS_H

#include <algorithm>
#include <unordered_map>
#include "graph.hpp"
#include "values.hpp"
#include "function_alias.hpp"

namespace stardec {
    typedef <typename T>
    class update_function {
    public:
        update_function(bool isfinal) : _final(isfinal) {}
        bool is_final() const {return _final;}

        virtual T update(argument *arg) const = 0;

    private:
        bool _final;
    };


    /***************** belief_update ********************/
    class belief_update : public update_function<belief> {
    public:
        belief_update(belief_function bel, bool isfinal) : update_function(isfinal), _fct(bel) {}

        virtual belief update(const argument &arg) const override {
            return _fct(arg);
        }

    private:
        belief_function _fct;
    };

    inline void fast_refine(argument *arg, bool side, double factor) {
        double old_belief = arg->value(stardec::belief).value();
        if(side)
            arg->value(stardec::belief) = old_belief + factor * (1 - old_belief);
        else
            arg->value(stardec::belief) = old_belief * (1 - factor);
    }

    bool attackers_below_half_belief(const argument &arg) {
        auto atkers = arg.get_attackers();
        return std::all_of(atkers.cbegin(), atkers.cend(), [](auto atk){atk.second.value(stardec::belief).value() <= 0.5;});
    }

    belief fast_general_update(const argument &arg, double factor) {
        if(attackers_below_half_belief(arg)) {
            fast_refine(*arg, true, factor);
            for(auto rel : arg.get_attacked())
                fast_refine(rel.second, false, factor);
        }
        return arg.value(stardec::belief);
    }

    belief fast_ambivalent(const argument &arg) {
        return fast_general_update(arg, 0.75);
    }

    belief fast_strict(const argument &arg) {
        return fast_general_update(arg, 1.0);
    }
    /***************** belief_update ********************/
}

#endif //STARDEC_UPDATE_FUNCTIONS_H
