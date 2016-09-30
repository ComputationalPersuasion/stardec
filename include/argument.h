#ifndef STARDEC_ARGUMENT_H
#define STARDEC_ARGUMENT_H

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace stardec {
    class Graph;

    class Argument {
    public:
        Argument(std::string label, unsigned int id);

        unsigned int get_component() const {return _component;}
        void set_component(unsigned int component) {_component = component;}
        unsigned int id() const {return _id;}
        const std::string &label() const {return _label;}
        const std::map<unsigned int, std::shared_ptr<Argument>> get_attackers() const {return _is_atked_by;}
        const std::map<unsigned int, std::shared_ptr<Argument>> get_attacked() const {return _attacks;}

        void reset_belief();

        void propagate_component();

        void add_attack_to(const std::shared_ptr<Argument> &atked);
        void add_attack_from(const std::shared_ptr<Argument> &attacker);

        bool is_acceptable(const Graph &graph, const std::vector<std::string> &execution) const;

    private:
        std::string _label;
        unsigned int _id, _component;
        double _belief, _initial_belief;
        std::map<unsigned int, std::shared_ptr<Argument>> _attacks, _is_atked_by;
    };

    enum state {OUT, IN, UNDEC};
    void change_argument_state(std::unordered_map<std::string, state> &map, const Graph &graph, const std::string &arg, const std::string &entrypoint);
}


#endif //STARDEC_ARGUMENT_H
