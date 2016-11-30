#ifndef STARDEC_UPDATE_FUNCTIONS_H
#define STARDEC_UPDATE_FUNCTIONS_H

#include <algorithm>
#include "distribution.h"
#include "graph.h"

namespace stardec {
    inline void naive(splittercell::distribution &dist, const argument &arg) {
        dist.refine(arg.id(), true, 1.0);
    }

    void trusting(splittercell::distribution &dist, const argument &arg) {
        naive(dist, arg);
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
