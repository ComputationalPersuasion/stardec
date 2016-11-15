#ifndef STARDEC_GRAPH_H
#define STARDEC_GRAPH_H

#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <functional>
#include "argument.h"
#include "logicaloperator.h"
#include "distribution.h"

namespace stardec {
    class graph {
    public:
        graph() : _distribution({}) {}
        void add_argument(const std::shared_ptr<argument>& argument);
        const std::shared_ptr<argument>& arg(const std::string &label) const {return _arguments.at(label);}
        const unsigned int id_of(const std::string &label) const {return arg(label)->id();}
        const std::unordered_map<std::string, std::shared_ptr<argument>>& arguments() const {return _arguments;};
        std::set<std::string> arguments_labels() const;
        void attack(const std::string &arg1, const std::string &arg2);
        void set_goals(logicaloperator *tree);
        void set_distribution(const std::vector<std::shared_ptr<splittercell::flock>> &flocks);
        const splittercell::distribution& distribution() const {return _distribution;}

    private:
        std::function<bool(const std::vector<std::string>&)> build_goal_tree(logicaloperator *tree);

        std::unordered_map<std::string, std::shared_ptr<argument>> _arguments;
        std::function<bool(std::vector<std::string>)> _goalformula;
        splittercell::distribution _distribution;
    };
}

#endif //STARDEC_GRAPH_H
