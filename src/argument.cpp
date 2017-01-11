#include <unordered_map>
#include <functional>
#include <algorithm>
#include "argument.h"
#include "graph.h"

namespace stardec {
    void change_argument_state(std::unordered_map<std::string, state> &map, const graph &g, const std::string &arg, const std::string &entrypoint) {
        auto argument = g.arg(arg);
        auto attackers = argument->get_attackers();
        bool propagate = false;
        auto is_in = [&map](auto &arg){auto lbl = arg.second.lock()->label(); return (map.find(lbl) != map.cend()) && (map[lbl] == IN);};
        auto is_undec = [&map](auto &arg){auto lbl = arg.second.lock()->label(); return (map.find(lbl) != map.cend()) && (map[lbl] == UNDEC);};
        if( std::any_of(attackers.cbegin(), attackers.cend(), is_in) ) {
            if(map[arg] != OUT) {
                map[arg] = OUT;
                propagate = true;
            }
        } else if( std::any_of(attackers.cbegin(), attackers.cend(), is_undec) ) {
            if(map[arg] != UNDEC) {
                map[arg] = UNDEC;
                propagate = true;
            }
        } else /*if((map.find(arg) != map.cend()) && (map[arg] != IN)) */{
            if(map[arg] != IN) {
                map[arg] = IN;
                propagate = true;
            }
        }

        if(propagate) {
            for(auto attacked : argument->get_attacked()) {
                auto attacked_label = attacked.second.lock()->label();
                if(attacked_label == entrypoint && map[attacked_label] == IN && map[arg] == IN)
                    map[arg] = UNDEC;
                change_argument_state(map, g, attacked_label, entrypoint);
            }
        }
    }

    argument::argument(std::string label, unsigned int id) {
        _label = label;
        _id = id;
        _initial_belief = _belief = 0.5;
    }

    void argument::reset_belief() {
        _belief = _initial_belief;
    }

    void argument::propagate_component() {
        for (auto a : _attacks) {
            auto ptr = a.second.lock();
            if (ptr->get_component() != this->_component) {
                ptr->set_component(this->_component);
                ptr->propagate_component();
            }
        }

        for (auto a : _is_atked_by) {
            auto ptr = a.second.lock();
            if (ptr->get_component() != this->_component) {
                ptr->set_component(this->_component);
                ptr->propagate_component();
            }
        }
    }

    void argument::add_attack_to(const std::shared_ptr<argument> &atked) {
        _attacks[atked->_id] = atked;
    }

    void argument::add_attack_from(const std::shared_ptr<argument> &attacker) {
        _is_atked_by[attacker->_id] = attacker;
    }

    bool argument::is_acceptable(const graph &g, const std::vector<std::string> &execution) const {
        std::unordered_map<std::string, state> statemap;
        for(auto arg : execution)
            change_argument_state(statemap, g, arg, arg);
        return statemap[_label] == IN;
    }
}
