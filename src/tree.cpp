#include <memory>
#include <vector>
#include <future>
#include "tree.h"

namespace stardec {
    std::shared_ptr<leafnode> build_execution(const graph &g, const std::string &arg, std::vector<std::string> execution,
                                              const std::vector<filterfunction> filter_functions, unsigned int horizon) {
        std::set<std::string> args = g.arguments_labels();
        execution.push_back(arg);
        for(auto filter : filter_functions)
            filter(args, execution);
        if(args.empty())
            return std::make_shared<leafnode>(0, arg); //TEMP FOR COMPILATION
        auto n = std::make_shared<node>(arg);
        for(auto nextarg : args) {
            n->add_child(build_execution(g, nextarg, execution, filter_functions, horizon));
        }
        return n;
    }

    tree::tree(const graph &g, std::vector<filterfunction> filter_functions, unsigned int horizon) {
        _root = std::make_unique<node>("");

        std::set<std::string> args = g.arguments_labels();
        std::vector<std::string> execution;
        for(auto filter : filter_functions)
            filter(args, execution);
        std::vector<std::future<std::shared_ptr<leafnode>>> children;
        for(auto arg : args)
            children.push_back(std::async(std::launch::async, build_execution, std::cref(g), std::cref(arg),
                                          execution, std::cref(filter_functions), horizon));
        for(auto &child : children)
            _root->add_child(child.get());
    }
}
