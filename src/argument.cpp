#include <unordered_map>
#include <functional>
#include <algorithm>
#include "argument.h"
#include "graph.h"

namespace stardec {
    void change_argument_state(std::unordered_map<std::string, state> &map, const Graph &graph, const std::string &arg, const std::string &entrypoint) {
        auto argument = graph.argument(arg);
        auto attackers = argument->get_attackers();
        bool propagate = false;
        if( std::any_of(attackers.cbegin(), attackers.cend(), [&map](auto &arg){return map[arg.second->get_label()] == IN;}) ) {
            if(map[arg] != OUT) {
                map[arg] = OUT;
                propagate = true;
            }
        } else if( std::any_of(attackers.cbegin(), attackers.cend(), [&map](auto &arg){return map[arg.second->get_label()] == UNDEC;}) ) {
            if(map[arg] != UNDEC) {
                map[arg] = UNDEC;
                propagate = true;
            }
        } else if(map[arg] != IN) {
            map[arg] = IN;
            propagate = true;
        }

        if(propagate) {
            for(auto attacked : argument->get_attacked()) {
                auto attacked_label = attacked.second->label();
                if(attacked_label == entrypoint && map[attacked_label] == IN && map[arg] == IN)
                    map[arg] = UNDEC;
                change_argument_state(map, graph, attacked_label, entrypoint);
            }
        }
    }

    Argument::Argument(std::string label, unsigned int id) {
        _label = label;
        _id = id;
        _initial_belief = _belief = 0.5;
    }

    void Argument::reset_belief() {
        _belief = _initial_belief;
    }

    void Argument::propagate_component() {
        for (auto a : _attacks)
            if (a.second->get_component() != this->_component) {
                a.second->set_component(this->_component);
                a.second->propagate_component();
            }

        for (auto a : _is_atked_by)
            if (a.second->get_component() != this->_component) {
                a.second->set_component(this->_component);
                a.second->propagate_component();
            }
    }

    void Argument::add_attack_to(const std::shared_ptr<Argument> &atked) {
        _attacks[atked->_id] = atked;
    }

    void Argument::add_attack_from(const std::shared_ptr<Argument> &attacker) {
        _is_atked_by[attacker->_id] = attacker;
    }

    bool Argument::is_acceptable(const Graph &graph, const std::vector<std::string> &execution) const {
        std::unordered_map<std::string, state> statemap;
        for(auto arg : execution)
            change_argument_state(statemap, graph, arg, arg);
        return statemap[_label] == IN;
    }
}



