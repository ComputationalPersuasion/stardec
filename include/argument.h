#ifndef STARDEC_ARGUMENT_H
#define STARDEC_ARGUMENT_H

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace stardec {
    class graph;

    class argument {
    public:
        argument(std::string label, unsigned int id);

        unsigned int get_component() const {return _component;}
        void set_component(unsigned int component) {_component = component;}
        void add_word(const std::string &w) {_words.push_back(w);}
        const std::vector<std::string> &words() const {return _words;}
        unsigned int id() const {return _id;}
        const std::string &label() const {return _label;}
        const std::map<unsigned int, std::weak_ptr<argument>> get_attackers() const {return _is_atked_by;}
        const std::map<unsigned int, std::weak_ptr<argument>> get_attacked() const {return _attacks;}

        void reset_belief();

        void propagate_component();

        void add_attack_to(const std::shared_ptr<argument> &atked);
        void add_attack_from(const std::shared_ptr<argument> &attacker);

        bool is_acceptable(const graph &graph, const std::vector<std::string> &execution) const;

    private:
        std::string _label;
        unsigned int _id, _component;
        double _belief, _initial_belief;
        std::map<unsigned int, std::weak_ptr<argument>> _attacks, _is_atked_by;
        std::vector<std::string> _words;
    };

    enum state {OUT, IN, UNDEC};
    void change_argument_state(std::unordered_map<std::string, state> &map, const graph &graph, const std::string &arg, const std::string &entrypoint);
}


#endif //STARDEC_ARGUMENT_H
