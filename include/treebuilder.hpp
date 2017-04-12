#ifndef STARDEC_TREEBUILDER_H
#define STARDEC_TREEBUILDER_H

#include <future>
#include <stdexcept>
#include "function_alias.hpp"
#include "graph.hpp"
#include "tree.hpp"
#include "node.hpp"
#include "sequence.hpp"

namespace stardec {
    class treebuilder {
    public:
        template <typename... Value>
        static void build_tree(tree<Value...> &t, const graph<Value...> &g, const std::vector<filter_function<Value...>> &filter_functions,
                               unsigned int horizon, bool multi_th=true, bool verbose=false) {
            double time = 0;
            auto root = t.root();
            auto arguments = g.arguments();
            sequence<Value...> execution;

            for(auto filter : filter_functions)
                filter(arguments, execution);

            std::launch mt_type;
            if(multi_th)
                mt_type = std::launch::async;
            else
                mt_type = std::launch::deferred;

            std::vector<std::future<void>> futures;
            auto begin = std::chrono::high_resolution_clock::now();
            for(auto argument : arguments)
                futures.push_back(std::async(mt_type, build_execution<Value...>, std::cref(g), std::ref(root), argument, execution, std::cref(filter_functions), horizon));
            for(auto &f : futures)
                f.wait();
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration<double>(end-begin).count();

            if(verbose)
                std::cout << "Tree created in " << time << " s." << std::endl << std::endl;
        }

        template <typename... Value>
        static void build_execution(const graph<Value...> &g, leafnode<Value...> *currentnode, argument<Value...> *arg, sequence<Value...> execution,
                                    const std::vector<filter_function<Value...>> &filter_functions, unsigned int horizon)  {
            auto arguments = g.arguments();
            execution.add(arg);
            for(auto filter : filter_functions)
                filter(arguments, execution);
            if(arguments.empty() || (execution.size() == horizon))
                currentnode->add_child(std::make_unique<leafnode<Value...>>(arg->label()));
            else {
                auto n = std::make_unique<node<Value...>>(arg->label());
                for(auto nextarg : arguments)
                    build_execution(g, n.get(), nextarg, execution, filter_functions, horizon);
                currentnode->add_child(std::move(n));
            }
        }
    };
}

#endif //STARDEC_TREEBUILDER_H
