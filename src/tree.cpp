#include <memory>
#include <vector>
#include <thread>
#include <future>
#include "tree.h"

namespace stardec {
    std::shared_ptr<LeafNode> build_execution(const Graph &graph, const std::string &arg, std::vector<std::string> execution,
                                              const std::vector<FilterFunction> filter_functions, unsigned int horizon) {
        std::set<std::string> args = graph.arguments_labels();
        execution.push_back(arg);
        for(auto filter : filter_functions)
            filter(args, execution);
        if(args.empty())
            return std::make_shared<LeafNode>(get_value(), arg);
        auto node = std::make_shared<Node>(arg);
        for(auto nextarg : args) {
            node->add_child(build_execution(graph, nextarg, execution, filter_functions, horizon));
        }
        return node;
    }

    Tree::Tree(const stardec::Graph &graph, std::vector<FilterFunction> filter_functions, unsigned int horizon) {
        _root = std::make_unique<Node>("");

        std::set<std::string> args = graph.arguments_labels();
        std::vector<std::string> execution;
        for(auto filter : filter_functions)
            filter(args, execution);
        std::vector<std::future<std::shared_ptr<LeafNode>>> children;
        for(auto arg : args)
            children.push_back(std::async(std::launch::async, build_execution, std::cref(graph), std::cref(arg),
                                          execution, std::cref(filter_functions), horizon));
        for(auto child : children)
            _root->add_child(child.get());
    }
}
