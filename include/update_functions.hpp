#ifndef STARDEC_UPDATE_FUNCTIONS_H
#define STARDEC_UPDATE_FUNCTIONS_H

#include <algorithm>
#include "graph.hpp"

namespace stardec {
    inline void refine(argument *arg, bool side, double factor) {
        if(side)
            arg->set_belief(arg->belief() + factor * (1 - arg->belief()));
        else
            arg->set_belief(arg->belief() * (1 - factor));
    }

    inline void naive(const argument &arg) {
        refine(*arg, true, 1.0);
    }

    void trusting(const argument &arg) {
        naive(arg);
        for(auto rel : arg.get_attacked())
            dist.refine(rel.first, false, 1.0);
        for(auto rel : arg.get_attackers())
            dist.refine(rel.first, false, 1.0);
    }

    bool attackers_below_half(splittercell::distribution &dist, const argument &arg) {
        auto atkers = arg.get_attackers();
        std::vector<unsigned int> atkers_ids;
        std::transform(atkers.cbegin(), atkers.cend(), std::back_inserter(atkers_ids), [](auto p){return p.first;});
        auto beliefs = dist[atkers_ids];
        return std::all_of(beliefs.cbegin(), beliefs.cend(), [](auto p){return p.second <= 0.5;});
    }

    void fast_strict(splittercell::distribution &dist, const argument &arg) {
        if(attackers_below_half(dist, arg)) {
            dist.fast_refine(arg.id(), true, 1.0);
            for(auto rel : arg.get_attacked())
                dist.fast_refine(rel.first, false, 1.0);
        }
    }

    void fast_ambivalent(splittercell::distribution &dist, const argument &arg) {
        if(attackers_below_half(dist, arg)) {
            dist.fast_refine(arg.id(), true, 0.75);
            for(auto rel : arg.get_attacked())
                dist.fast_refine(rel.first, false, 0.75);
        }
    }

    void fast_ambivalent_with_filter(splittercell::distribution &dist, const argument &arg, const std::unordered_set<unsigned int> &filter) {
        if(filter.find(arg.id()) == filter.cend())
            fast_ambivalent(dist, arg);
    }

    void strict(splittercell::distribution &dist, const argument &arg) {
        if(attackers_below_half(dist, arg)) {
            dist.refine(arg.id(), true, 1.0);
            for(auto rel : arg.get_attacked())
                dist.refine(rel.first, false, 1.0);
        }
    }

    void ambivalent(splittercell::distribution &dist, const argument &arg) {
        if(attackers_below_half(dist, arg)) {
            dist.refine(arg.id(), true, 0.75);
            for(auto rel : arg.get_attacked())
                dist.refine(rel.first, false, 0.75);
        }
    }
}

#endif //STARDEC_UPDATE_FUNCTIONS_H
