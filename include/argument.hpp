#ifndef STARDEC_ARGUMENT_H
#define STARDEC_ARGUMENT_H

#include <typeinfo>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include "values.hpp"

namespace stardec {

    template <typename ...Value>
    class argument {
    public:
        argument(std::string label, unsigned int id) : _label(label), _id(id) {}

        void add_words(const std::vector<std::string> &words) {for(auto w : words) _words.push_back(w);}
        const std::vector<std::string> &words() const {return _words;}
        unsigned int id() const {return _id;}
        const std::string &label() const {return _label;}
        const std::map<unsigned int, argument*> get_attackers() const {return _is_atked_by;}
        const std::map<unsigned int, argument*> get_attacked() const {return _attacks;}
        template <typename T> const T &get() const { return std::get<T>(_values);}
        template <typename T> T &get() { return std::get<T>(_values);}

        void add_attack_to(argument *atked) {_attacks[atked->_id] = atked;}
        void add_attack_from(argument *attacker) {_is_atked_by[attacker->_id] = attacker;}

        //bool is_acceptable(const graph &graph, const std::vector<std::string> &execution) const;

    private:
        std::string _label;
        unsigned int _id;
        std::map<unsigned int, argument*> _attacks, _is_atked_by;
        std::vector<std::string> _words;
        std::tuple<Value...> _values;
    };

    //enum state {OUT, IN, UNDEC};
    //void change_argument_state(std::unordered_map<std::string, state> &map, const graph &graph, const std::string &arg, const std::string &entrypoint);
}

//#include "templates/argument.tpp"

#endif //STARDEC_ARGUMENT_H
