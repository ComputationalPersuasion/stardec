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

    void update_if_below_half(splittercell::distribution &dist, const argument &arg, double factor) {
        auto atkers = arg.get_attackers();
        std::vector<unsigned int> atkers_ids;
        std::transform(atkers.cbegin(), atkers.cend(), std::back_inserter(atkers_ids), [](auto p){return p.first;});
        auto beliefs = dist[atkers_ids];
        if(std::all_of(beliefs.cbegin(), beliefs.cend(), [](auto p){return p.second <= 0.5;})) {
            dist.refine(arg.id(), true, factor);
            for(auto rel : arg.get_attacked())
                dist.refine(rel.first, false, factor);
        }
    }

    void strict(splittercell::distribution &dist, const argument &arg) {
        update_if_below_half(dist, arg, 1.0);
    }

    void ambivalent(splittercell::distribution &dist, const argument &arg) {
        update_if_below_half(dist, arg, 0.75);
    }
}

#endif //STARDEC_UPDATE_FUNCTIONS_H
