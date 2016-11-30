#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <future>
#include "tree.h"
#include "distribution.h"

namespace stardec {
    std::shared_ptr<leafnode> build_execution(const graph &g, const std::string &arg, std::vector<std::string> execution,
                                              const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
                                              const valuationfunction &valuation_function, const aggregationfunction &aggregation_function,
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
                update_function(dist, *(g.arg(a)));
            auto values = valuation_function(g, execution);
            auto goals = g.goals();
            std::vector<unsigned int> goalsids(goals.size());
            std::transform(goals.cbegin(), goals.cend(), goalsids.begin(), [](auto a){return a->id();});
            auto beliefs = dist[goalsids];
            return std::make_shared<leafnode>(aggregation_function(values, beliefs), arg);
        }
        auto n = std::make_shared<node>(arg);
        for(auto nextarg : args)
            n->add_child(build_execution(g, nextarg, execution, filter_functions, update_function, valuation_function, aggregation_function, horizon, mt));
        return n;
    }

    tree::tree(const graph &g, const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
               const valuationfunction &valuation_function, const aggregationfunction &aggregation_function,
               unsigned int horizon, bool verbose, bool mtbuild, bool mtdist) {
        double time = 0;
        _root = std::make_shared<node>("");
        std::vector<std::future<std::shared_ptr<leafnode>>> futures;

        std::set<std::string> args = g.arguments_labels();
        std::vector<std::string> execution;
        for(auto filter : filter_functions)
            filter(args, execution);
        if(mtbuild) {
            auto begin = std::chrono::high_resolution_clock::now();
            for(auto arg : args)
                futures.push_back(std::async(std::launch::async, build_execution, std::cref(g), std::cref(arg), execution, std::cref(filter_functions),
                                              std::cref(update_function), std::cref(valuation_function), std::cref(aggregation_function), horizon, mtdist));
            for(auto &f : futures)
                _root->add_child(f.get());
            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration<double>(end-begin).count();
        } else {
            for(auto arg : args) {
                auto begin = std::chrono::high_resolution_clock::now();
                _root->add_child(build_execution(g, arg, execution, filter_functions, update_function, valuation_function, aggregation_function, horizon, mtdist));
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

    void build_children(std::stringstream &s, const std::string &parent, const std::shared_ptr<leafnode> &node, bool square, unsigned int &id) {
        std::string p = "node" + std::to_string(++id), shape, edge;
        s << p;
        if(node->is_leaf())
            shape = "none, label=\"" + std::accumulate(node->values.cbegin(), node->values.cend(), std::string(), [](auto acc, auto v){return acc + "," + std::to_string(v);}) + "\"]";
        else
            shape = (square ? ("square" + (node->optimal != nullptr ? ", label=\"" + node->optimal->label + "\"]" : "]")) : "circle]");
        edge  = " [label=\"" + node->label + "\"]";
        s << " [shape=" << shape << std::endl;
        s << parent << " -- " << p << edge << std::endl;

        for(auto c : node->children)
            build_children(s, p, c, !square, id);
    }

    std::string tree::to_dot() const {
        unsigned int id = 0;
        std::stringstream s;
        s << "graph tree {" << std::endl;
        s << "rankdir=LR" << std::endl;
        s << "root [shape=square" << (_root->optimal != nullptr ? ", label=\"" + _root->optimal->label + "\"": "") << "]" << std::endl;
        for(auto c : _root->children)
            build_children(s, "root", c, false, id);
        s << "}" << std::endl;
        return s.str();
    }

    void tree::optimize(const decisionfunction &decision_function) {
        decision_function(_root);
    }
}
