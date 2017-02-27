#ifndef STARDEC_GRAPH_H
#define STARDEC_GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <memory>
#include <functional>
#include "argument.hpp"

namespace stardec {
    class graph {
    public:
        graph() : _goal(nullptr) {}

        const std::shared_ptr<argument>& arg(const std::string &label) const {return _arguments.at(label);}
        const std::unordered_map<std::string, std::shared_ptr<argument>>& argument_map() const {return _arguments;}
        std::set<argument*> arguments() const {
            std::set<argument*> args;
            std::transform(_arguments.cbegin(), _arguments.cend(), std::inserter(args), [](auto p) {return p.second.get();});
            return args;
        }
        std::set<std::string> arguments_labels() const {return _labels();}
        const argument* const &goal() const {return _goal;}

        void add_argument(const std::shared_ptr<argument>& argument) {_arguments[arg->label()] = arg; _labels.push_back(arg->label());}
        //const unsigned int id_of(const std::string &label) const {return arg(label)->id();}
        void attack(const std::string &arg1, const std::string &arg2) {
            _arguments[arg1]->add_attack_to(_arguments[arg2]);
            _arguments[arg2]->add_attack_from(_arguments[arg1]);
        }
        void set_goal(const std::string &goal) {_goal = arg(goal).get();};


    private:
        argument* _goal;
        std::unordered_map<std::string, std::shared_ptr<argument>> _arguments;
        std::vector<std::string> _labels;
    };
}

#endif //STARDEC_GRAPH_H
