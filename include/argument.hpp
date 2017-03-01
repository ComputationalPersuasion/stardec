#ifndef STARDEC_ARGUMENT_H
#define STARDEC_ARGUMENT_H

#include <typeinfo>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <utility>

namespace stardec {

    template <typename ...Value>
    class argument {
    public:
        argument(std::string label, unsigned int id, std::tuple<Value...> values) : _label(label), _id(id), _values(values) {
            for(unsigned int i = 0; i < values.size(); i++) {
                auto val = std::get<i>(values);
                _mapping[typeid(val)] = val;
            }
        }

        //double belief() const {return _belief;}
        //void set_belief(double belief) {_belief = belief;}

        //void add_word(const std::string &w) {_words.push_back(w);}
        //const std::vector<std::string> &words() const {return _words;}
        unsigned int id() const {return _id;}
        const std::string &label() const {return _label;}
        const std::map<unsigned int, argument*> get_attackers() const {return _is_atked_by;}
        const std::map<unsigned int, argument*> get_attacked() const {return _attacks;}
        Value& value(std::type_index type) const {return _mapping.at(type);}

        //void reset_belief();

        void add_attack_to(argument *atked) {_attacks[atked->_id] = atked;}
        void add_attack_from(argument *attacker) {_is_atked_by[attacker->_id] = attacker;}

        //bool is_acceptable(const graph &graph, const std::vector<std::string> &execution) const;

    private:
        std::string _label;
        unsigned int _id;
        std::map<unsigned int, argument*> _attacks, _is_atked_by;
        //std::vector<std::string> _words;
        std::tuple<Value...> _values;
        std::unordered_map<std::type_index, unsigned int> _mapping;
    };

    //enum state {OUT, IN, UNDEC};
    //void change_argument_state(std::unordered_map<std::string, state> &map, const graph &graph, const std::string &arg, const std::string &entrypoint);
}


#endif //STARDEC_ARGUMENT_H
