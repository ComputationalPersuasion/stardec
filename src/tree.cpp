#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>
#include "tree.h"
#include "distribution.h"

namespace stardec {
    std::shared_ptr<leafnode> build_execution(const graph &g, const std::string &arg, std::vector<std::string> execution,
                                              const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
                                              const valuationfunction &valuation_function, const aggregationfunction &aggregation_function,
                                              unsigned int horizon) {
        auto args = g.arguments_labels();
        execution.push_back(arg);
        for(auto filter : filter_functions)
            filter(args, execution);
        if(args.empty()) {
            splittercell::distribution dist(g.distribution());
            update_function(dist, *(g.arg(arg)));
            auto values = valuation_function(g, execution);
            auto goals = g.goals();
            std::vector<unsigned int> goalsids;
            std::transform(goals.cbegin(), goals.cend(), std::back_inserter(goalsids), [](auto a){return a->id();});
            auto beliefs = dist[goalsids];
            return std::make_shared<leafnode>(aggregation_function(values, beliefs), arg);
        }
        auto n = std::make_shared<node>(arg);
        for(auto nextarg : args)
            n->add_child(build_execution(g, nextarg, execution, filter_functions, update_function, valuation_function, aggregation_function, horizon));
        return n;
    }

    tree::tree(const graph &g, const std::vector<filterfunction> &filter_functions, const updatefunction &update_function,
               const valuationfunction &valuation_function, const aggregationfunction &aggregation_function,
               unsigned int horizon) {
        _root = std::make_unique<node>("");

        std::set<std::string> args = g.arguments_labels();
        std::vector<std::string> execution;
        for(auto filter : filter_functions)
            filter(args, execution);
        for(auto arg : args)
            _root->add_child(build_execution(g, arg, execution, filter_functions, update_function, valuation_function, aggregation_function, horizon));
    }

    void build_children(std::stringstream &s, const std::string &parent, const std::shared_ptr<leafnode> &node, bool square, unsigned int &id) {
        std::string p = "node" + std::to_string(++id), shape, edge;
        s << p;
        if(node->is_leaf())
            shape = "none, label=\"" + std::to_string(node->value) + "\"]";
        else {
            shape = (square ? "square]" : "circle]");
            edge  = " [label=\"" + node->label + "\"]";
        }

        s << " [shape=" << shape << std::endl;
        s << parent << " -- " << p << edge << std::endl;

        for(auto c : node->children)
            build_children(s, p, c.second, !square, id);
    }

    std::string tree::to_dot() const {
        unsigned int id = 0;
        std::stringstream s;
        s << "graph tree {" << std::endl;
        s << "rankdir=LR" << std::endl;
        s << "root [shape=square]" << std::endl;
        for(auto c : _root->children)
            build_children(s, "root", c.second, false, id);
        s << "}" << std::endl;
        return s.str();
    }
}
