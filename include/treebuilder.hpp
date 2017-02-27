#ifndef STARDEC_TREEBUILDER_H
#define STARDEC_TREEBUILDER_H

#include <stdexcept>
#include "graph.hpp"
#include "tree.hpp"
#include "node.hpp"

namespace stardec {
    template<typename... Values>
    class treebuilder {
    public:
        static tree<Values...> build_tree(const graph &g, const std::vector<filterfunction> &filter_functions) {
            tree<Values...> t;
            double time = 0;
            auto root = t.root();
            auto arguments = g.arguments();
            std::vector<argument*> execution;

            for(auto filter : filter_functions)
                filter(args, execution);

            if(mtbuild) {
                std::vector<std::future<std::shared_ptr<leafnode<Values...>>>> futures;
                auto begin = std::chrono::high_resolution_clock::now();
                for(auto arg : args)
                    futures.push_back(std::async(std::launch::async, build_execution<Values...>, std::cref(g), arg, execution, std::cref(filter_functions),
                                                  horizon, mtdist));
                for(auto &f : futures)
                    _root->add_child(f.get());
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration<double>(end-begin).count();
            } else {
                for(auto arg : args) {
                    auto begin = std::chrono::high_resolution_clock::now();
                    _root->add_child(build_execution<Values...>(g, arg, execution, filter_functions, horizon, mtdist));
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

        static std::shared_ptr<leafnode<Values...>> build_execution(const graph &g, const std::string &arg, std::vector<argument*> execution,
                                                  const std::vector<filterfunction> &filter_functions, unsigned int horizon, bool mt)  {
            auto arguments = g.arguments();
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
    };
}

#endif //STARDEC_TREEBUILDER_H
