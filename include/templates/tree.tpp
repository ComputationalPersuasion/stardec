#ifndef STARDEC_TREE_TEMPLATE_H
#define STARDEC_TREE_TEMPLATE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include "function_alias.h"
#include "tree.h"
#include "node.h"

namespace stardec {
    template <typename Belief, typename... Values>
    std::shared_ptr<leafnode<Belief, Values...>> build_execution(const graph &g, const std::string &arg, std::vector<std::string> execution,
                                              const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
                                              const std::tuple<valuationfunction<Values>...> &valuation_function, //const aggregationfunction<T> &aggregation_function,
                                              unsigned int horizon, bool mt) {
        auto args = g.arguments_labels();
        execution.push_back(arg);
        for(auto filter : filter_functions)
            filter(args, execution);
        if(args.empty() || (execution.size() == horizon)) {
            splittercell::distribution dist(g.distribution());
            if(!mt)
                dist.disable_mt();
            for(auto a : execution)
                update_function(dist, *g.arg(a));
            auto values = valuation_function(g, execution);
            auto goals = g.goals();
            std::vector<unsigned int> goalsids(goals.size());
            std::transform(goals.cbegin(), goals.cend(), goalsids.begin(), [](auto a){return a->id();});
            auto beliefs = dist[goalsids];
            return std::make_shared<leafnode<Belief, Values...>>(aggregation_function(values, beliefs), arg);
        }
        auto n = std::make_shared<node<Belief, Values...>>(arg);
        for(auto nextarg : args)
            n->add_child(build_execution<Belief, Values...>(g, nextarg, execution, filter_functions, update_function, valuation_function, /*aggregation_function,*/ horizon, mt));
        return n;
    }

    template <typename Belief, typename... Values>
    void tree::build_execution(std::shared_ptr<leafnode<Belief, Values...> parent, const std::string &arg, std::vector<std::string> execution,
                              std::shared_ptr<leafnode<Belief, Values...>> &current_best) {
        auto goals = _graph->goals();
        unsigned int num_goals = goals.size();
        auto args = _graph->arguments_labels();
        for(auto &filter : _f_struct->filters)
            filter(args, execution);

        if(!args.empty() && execution.size() < (_horizon - 1)) {
            for(auto nextarg : args) {
                std::tuple<Belief, Values...> values;
                std::vector<std::string> extended_execution(execution);
                extended_execution.push_back(nextarg);
                splittercell::distribution dist(_graph->distribution(), _mtdist);
                _f_struct->update(dist, *g.arg(a));

                for(unsigned int i = 0; i < num_goals; i++)
                    std::get<i>(values) = dist[goals[i]];
                for(unsigned int i = 0; i < _f_struct->valuation.size(); i++)
                    std::get<i+num_goals>(values) = _f_struct->valuation[i](*_graph, extended_execution);
            }
        }
    }

    template <typename Belief, typename... Values>
    void tree::build_and_optimize_with_heuristics(bool verbose) {
        std::vector<std::future<std::shared_ptr<leafnode<Belief, Values...>>>> futures;

        statistics::start_chrono();
        _root = std::make_shared<node<Belief, Values...>>("");
        build_execution(_root, "", {}, nullptr);
        //FINISH HERE
    }

    template <typename Belief, typename... Values>
    tree<Belief, Values...>::tree(std::shared_ptr<graph> g, std::shared_ptr<function_struct<Belief, Values...>> f_struct, unsigned int horizon, bool mtbuild, bool mtdist) : _graph(g), _f_struct(f_struct), _horizon(horizon), _mtbuild(mtbuild), _mtdist(mtdist) {
        if(g.goals().empty())
            std::cout << "NO GOAL DEFINED!" << std::endl;
    }

    template <typename Belief, typename... Values>
    tree<Belief, Values...>::tree(const graph &g, const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
               const std::tuple<valuationfunction<Values>...> &valuation_function, //const aggregationfunction<T> &aggregation_function,
               unsigned int horizon, bool verbose, bool mtbuild, bool mtdist) {
        if(g.goals().empty())
            std::cout << "NO GOAL DEFINED!" << std::endl;

        double time = 0;
        _root = std::make_shared<node<Belief, Values...>>("");
        std::vector<std::future<std::shared_ptr<leafnode<Belief, Values...>>>> futures;

        std::set<std::string> args = g.arguments_labels();
        std::vector<std::string> execution;
        for(auto filter : filter_functions)
            filter(args, execution);
        if(mtbuild) {
            auto begin = std::chrono::high_resolution_clock::now();
            for(auto arg : args)
                futures.push_back(std::async(std::launch::async, build_execution<Belief, Values...>, std::cref(g), arg, execution, std::cref(filter_functions),
                                              std::cref(update_function), std::cref(valuation_function), /*std::cref(aggregation_function),*/ horizon, mtdist));
            for(auto &f : futures)
                _root->add_child(f.get());
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration<double>(end-begin).count();
        } else {
            for(auto arg : args) {
                auto begin = std::chrono::high_resolution_clock::now();
                _root->add_child(build_execution<Belief, Values...>(g, arg, execution, filter_functions, update_function, valuation_function, /*aggregation_function,*/ horizon, mtdist));
                auto end = std::chrono::high_resolution_clock::now();
                double t = std::chrono::duration<double>(end-begin).count();
                time += t;
                if(verbose)
                    std::cout << "Subtree " << arg << " treated in " << t << " s." << std::endl;
            }
        }
        if(verbose)
            std::cout << "Tree created in " << time << " s." << std::endl << std::endl;
    }
}

#endif //STARDEC_TREE_TEMPLATE_H
