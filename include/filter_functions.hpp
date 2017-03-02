#ifndef STARDEC_FILTER_FUNCTIONS_H
#define STARDEC_FILTER_FUNCTIONS_H

#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "graph.hpp"

namespace stardec {
    /*bool is_attacking_goal(const graph &g, const argument* const candidate, const std::vector<argument*> &sequence) {
        if((sequence.size() % 2) != 0) return false;
        auto atkers = g->goal()->get_attackers();
        return atkers.find(candidate->id()) != atkers.cend();
    }*/

    template <typename... Value>
    void remove_goal_atks(const graph<Value...> &g, std::set<argument<Value...>*> &candidates, const std::vector<argument<Value...>*> &sequence) {
        if((sequence.size() % 2) != 0)
            return;
        for(auto a : g.goal()->get_attackers())
            candidates.erase(a.second);
    }

    /*bool is_duplicate(const argument* const candidate, const std::vector<argument*> &sequence) {
        return std::find(sequence.cbegin(), sequence.cend(), candidate) != sequence.cend();
    }*/

    //Cannot use set_difference because it requires the containers to be sorted (and we cannot sort the execution)
    template <typename... Value>
    void remove_duplicate(std::set<argument<Value...>*> &candidates, const std::vector<argument<Value...>*> &sequence) {
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

    template <typename... Value>
    void relevant(std::set<argument<Value...>*> &candidates, const std::vector<argument<Value...>*> &sequence) {
        if(sequence.empty())
            return;
        std::set<argument<Value...>*> relevant, relevant_candidates;
        for(auto posited : sequence) {
            auto atkers = posited->get_attackers();
            for(auto atk : atkers)
                relevant.insert(atk.second);
        }
        for(auto posited : sequence)
            relevant.erase(posited);
        if(relevant.empty())
            return;

        std::copy_if(candidates.cbegin(), candidates.cend(), std::inserter(relevant_candidates, relevant_candidates.end()), [&relevant](auto c) {return relevant.find(c) != relevant.cend();});
        candidates = relevant_candidates;
    }

    template <typename... Value>
    void enforce_goal(const graph<Value...> &g, std::set<argument<Value...>*> &candidates, const std::vector<argument<Value...>*> &sequence) {
        auto goal = g.goal();
        if(candidates.find(goal) == candidates.cend() && std::find(sequence.cbegin(), sequence.cend(), goal) == sequence.cend())
            candidates.clear();
    }
}

#endif //STARDEC_FILTER_FUNCTIONS_H
