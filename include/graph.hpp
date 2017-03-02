#ifndef STARDEC_GRAPH_H
#define STARDEC_GRAPH_H

#include <stdexcept>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include "argument.hpp"

namespace stardec {
    template <typename ...Value>
    class graph {
    public:
        graph() : _goal(nullptr) {}

        const std::shared_ptr<argument<Value...>>& arg(const std::string &label) const {return _arguments.at(label);}
        const std::unordered_map<std::string, std::shared_ptr<argument<Value...>>>& argument_map() const {return _arguments;}
        std::set<argument<Value...>*> arguments() const {
            std::set<argument<Value...>*> args;
            std::transform(_arguments.cbegin(), _arguments.cend(), std::inserter(args, args.end()), [](auto p) {return p.second.get();});
            return args;
        }
        std::set<std::string> arguments_labels() const {return _labels();}
        unsigned int number_of_arguments() const {return _arguments.size();}
        argument<Value...> *goal() const {return _goal;}

        void add_argument(const std::shared_ptr<argument<Value...>>& argument) {_arguments[argument->label()] = argument; _labels.push_back(argument->label());}
        void attack(argument<Value...> *arg1, argument<Value...> *arg2) {
            arg1->add_attack_to(arg2);
            arg2->add_attack_from(arg1);
        }
        void attack(const std::string &arg1, const std::string &arg2) {
            attack(_arguments.at(arg1).get(), _arguments.at(arg2).get());
        }
        void set_goal(const std::string &goal) {_goal = _arguments.at(goal).get();};

    private:
        argument<Value...>* _goal;
        std::unordered_map<std::string, std::shared_ptr<argument<Value...>>> _arguments;
        std::vector<std::string> _labels;
    };
}

#endif //STARDEC_GRAPH_H
