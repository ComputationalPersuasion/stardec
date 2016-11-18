#ifndef STARDEC_FILTER_FUNCTIONS_H
#define STARDEC_FILTER_FUNCTIONS_H

#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace stardec {
    //Consider all goals, do not use with logical NOTs.
    void remove_goal_atks(const graph &g, std::set<std::string> &considering, const std::vector<std::string> &current_exec) {
        if((current_exec.size() % 2) != 0)
            return;
        auto goals = g.goals();
        for(auto g : goals)
            for(auto a : g->get_attackers())
                considering.erase(a.second->label());
    }

    //Cannot use set_difference because it requires the containers to be sorted (and we cannot sort the execution)
    void remove_duplicate(std::set<std::string> &considering, const std::vector<std::string> &current_exec) {
        for(auto a : current_exec)
            considering.erase(a);
    }
}

#endif //STARDEC_FILTER_FUNCTIONS_H
