#include <set>
#include <algorithm>
#include "graph.h"

namespace stardec {
    void graph::set_distribution(const std::vector<std::shared_ptr<splittercell::flock>> &flocks) {
      _distribution = splittercell::distribution(flocks);
    }

    void graph::add_argument(const std::shared_ptr<argument> &arg) {
        _arguments[arg->label()] = arg;
    }

    void graph::attack(const std::string &arg1, const std::string &arg2) {
        _arguments[arg1]->add_attack_to(_arguments[arg2]);
        _arguments[arg2]->add_attack_from(_arguments[arg1]);
    }

    void graph::set_goals(logicaloperator *tree) {
        _goalformula = build_goal_tree(tree);
        delete tree;
    }

    std::function<bool(const std::vector<std::string>&)> graph::build_goal_tree(logicaloperator *tree) {
        std::function<bool(const std::vector<std::string>&)> ret;
        if(tree == nullptr) return nullptr;
        if(tree->left == nullptr && tree->right == nullptr)
            ret = std::bind(&argument::is_acceptable, std::cref(arg(tree->arg)), std::cref(*this), std::placeholders::_1);
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

    std::set<std::string> graph::arguments_labels() const {
        std::set<std::string> args;
        std::transform(arguments().cbegin(), arguments().cend(), std::inserter(args, args.end()),
                       [](auto pair){return pair.second->label();});
        return args;
    }
}
