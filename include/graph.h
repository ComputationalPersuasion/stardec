#ifndef STARDEC_GRAPH_H
#define STARDEC_GRAPH_H

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "argument.h"
#include "logicaloperator.h"

namespace stardec {
    class Graph {
    public:
        void add_argument(const std::shared_ptr<Argument>& argument);
        const std::shared_ptr<Argument>& argument(const std::string &label) const {return _arguments.at(label);}
        const std::unordered_map<std::string, std::shared_ptr<Argument>>& arguments() const {return _arguments;};
        std::set<std::string> arguments_labels() const;
        void attack(const std::string &arg1, const std::string &arg2);
        void set_goals(LogicalOperator *tree);

    private:
        std::function<bool(std::vector<std::string>)> build_goal_tree(LogicalOperator *tree);

        std::unordered_map<std::string, std::shared_ptr<Argument>> _arguments;
        std::function<bool(std::vector<std::string>)> _goalformula;
    };
}

#endif //STARDEC_GRAPH_H
