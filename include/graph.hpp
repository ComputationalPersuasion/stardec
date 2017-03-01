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
#include "parser/parser.hpp"
#include "parser/parser_struct.hpp"

void parse(const std::string &, stardec::parsed_item &, bool);

namespace stardec {
    template <typename ...Value>
    class graph {
    public:
        graph() : _goal(nullptr) {}
        graph(const std::string &filename, bool verbose=false) {
            std::ifstream f(filename);
            std::string line;
            parsed_item item;
            while(std::getline(f, line)) {
                if(line.empty()) continue;
                parse(line, item, verbose);
                switch(item.type) {
                    case ARG: use(item.arg.get()); break;
                    case WORDS: use(item.words.get()); break;
                    case GOAL: use(item.goal.get()); break;
                    case ATK: use(item.atk.get()); break;
                    default: throw std::domain_error("Undefined parsed type");
                }
            }
            f.close();
        }

        const std::shared_ptr<argument<Value...>>& arg(const std::string &label) const {return _arguments.at(label);}
        const std::unordered_map<std::string, std::shared_ptr<argument<Value...>>>& argument_map() const {return _arguments;}
        std::set<argument<Value...>*> arguments() const {
            std::set<argument<Value...>*> args;
            std::transform(_arguments.cbegin(), _arguments.cend(), std::inserter(args), [](auto p) {return p.second.get();});
            return args;
        }
        std::set<std::string> arguments_labels() const {return _labels();}
        argument<Value...> *goal() const {return _goal;}

        void add_argument(const std::shared_ptr<argument<Value...>>& argument) {_arguments[argument->label()] = argument; _labels.push_back(argument->label());}
        void attack(argument<Value...> *arg1, argument<Value...> *arg2) {
            arg1->add_attack_to(arg2);
            arg2->add_attack_from(arg1);
        }
        void attack(const std::string &arg1, const std::string &arg2) {
            attack(_arguments[arg1], _arguments[arg2]);
        }
        void set_goal(const std::string &goal) {_goal = arg(goal).get();};

        void use(parsed_argument *arg) {
            add_argument(std::make_shared<argument<Value...>>(arg->label, _arguments.size() + 1));
        }

        void use(parsed_words *w) {
            auto arg = _arguments.find(w->label);
            if(arg == _arguments.cend())
                throw std::invalid_argument("Argument " + w->label + " does not exist.");
            else {
                arg->second->add_words(w->words);
            }
        }

        void use(parsed_attack *atk) {
            auto from = _arguments.find(atk->from);
            if(from == _arguments.cend())
                throw std::invalid_argument("Argument " + atk->from + " does not exist.");
            auto to = _arguments.find(atk->to);
            if(to == _arguments.cend())
                throw std::invalid_argument("Argument " + atk->to + " does not exist.");
            attack(from->second.get(), to->second.get());
        }

        void use(parsed_goal *goal) {
            auto arg = _arguments.find(goal->label);
            if(arg == _arguments.cend())
                throw std::invalid_argument("Argument " + goal->label + " does not exist.");
            else {
                _goal = arg->second.get();
            }
        }

    private:
        argument<Value...>* _goal;
        std::unordered_map<std::string, std::shared_ptr<argument<Value...>>> _arguments;
        std::vector<std::string> _labels;
    };
}

#endif //STARDEC_GRAPH_H
