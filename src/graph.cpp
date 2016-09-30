#include <set>
#include <algorithm>
#include "graph.h"

namespace stardec {
    void Graph::add_argument(const std::shared_ptr<Argument> &argument) {
        _arguments[argument->label()] = argument;
    }

    void Graph::attack(const std::string &arg1, const std::string &arg2) {
        _arguments[arg1]->add_attack_to(_arguments[arg2]);
        _arguments[arg2]->add_attack_from(_arguments[arg1]);
    }

    void Graph::set_goals(LogicalOperator *tree) {
        _goalformula = build_goal_tree(tree);
        delete tree;
    }

    std::function<bool(std::vector<std::string>)> Graph::build_goal_tree(LogicalOperator *tree) {
        std::function<bool(std::vector<std::string>)> ret;
        if(tree == nullptr) return nullptr;
        if(tree->left == nullptr && tree->right == nullptr)
            ret = std::bind(&Argument::is_acceptable, std::cref(argument(tree->arg)), std::cref(*this), std::placeholders::_1);
        else {
            auto left = build_goal_tree(tree->left);
            auto right = build_goal_tree(tree->right);

            switch(tree->type) {
                case oper::NOT_OP:
                    ret = std::bind(std::logical_not<>{}, left); break;
                case oper::AND_OP:
                    ret = std::bind(std::logical_and<>{}, left, right); break;
                case oper::OR_OP:
                    ret = std::bind(std::logical_or<>{}, left, right); break;
                default: ret = nullptr;
            }
        }
        return ret;
    }

    std::set<std::string> Graph::arguments_labels() const {
        std::set<std::string> args;
        std::transform(arguments().begin(), arguments().end(), std::inserter(args, args.end()),
                       [](auto pair){return pair.second->label();});
        return args;
    }
}
