#ifndef STARDEC_FILTER_FUNCTIONS_H
#define STARDEC_FILTER_FUNCTIONS_H

#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "graph.h"

namespace stardec {
    /*bool is_attacking_goal(const graph &g, const argument* const candidate, const std::vector<argument*> &sequence) {
        if((sequence.size() % 2) != 0) return false;
        auto atkers = g->goal()->get_attackers();
        return atkers.find(candidate->id()) != atkers.cend();
    }*/

    void remove_goal_atks(const graph &g, std::set<argument*> &candidates, const std::vector<argument*> &sequence) {
        if((sequence.size() % 2) != 0)
            return;
        for(auto a : g.goal()->get_attackers())
            candidates.erase(a.second);
    }

    /*bool is_duplicate(const argument* const candidate, const std::vector<argument*> &sequence) {
        return std::find(sequence.cbegin(), sequence.cend(), candidate) != sequence.cend();
    }*/

    //Cannot use set_difference because it requires the containers to be sorted (and we cannot sort the execution)
    void remove_duplicate(std::set<argument*> &candidates, const std::vector<argument*> &current_exec) {
        for(auto a : sequence)
            candidates.erase(a);
    }

    /*bool is_relevant(const graph &g, const argument* const candidate, const std::vector<argument*> &sequence) {
        if(current_exec.empty())
            return true;
        else {
            std::set<argument*> relevant;
            for(auto posited : sequence) {
                auto atkers = posited.get_attackers();
                std::transform(atkers.cbegin(), atkers.cend(), std::inserter(relevant), [](auto atk){atk.second;});
            }
            if(relevant.empty())
                return true;
            else
                return relevant.find(candidate) != relevant.cend();
        }
    }*/

    void relevant(const graph &g, std::set<argument*> &candidates, const std::vector<argument*> &sequence) {
        if(sequence.empty())
            return;
        std::set<argument*> relevant, relevant_candidates;
        for(auto posited : sequence) {
            auto atkers = posited->get_attackers();
            for(auto atk : atkers)
                relevant.insert(atk.second);
        }
        for(auto posited : sequence)
            relevant.erase(posited);
        if(relevant.empty())
            return;

        std::copy_if(candidates.cbegin(), candidates.cend(), std::inserter(relevant_candidates), [&relevant](auto c) {return relevant.find(c) != relevant.cend();});
        candidates = relevant_candidates;
    }

    void enforce_goal(const graph &g, std::set<argument*> &candidates, const std::vector<argument*> &sequence) {
        auto goal = g.goal();
        if(candidates.find(goal) == candidates.cend() && std::find(sequence.cbegin(), sequence.cend(), goal) == sequence.cend())
            candidates.clear();
    }
}

#endif //STARDEC_FILTER_FUNCTIONS_H
