#ifndef STARDEC_TREE_H
#define STARDEC_TREE_H

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "graph.hpp"
#include "node.hpp"
#include "argument.hpp"
#include "function_alias.hpp"
//#include "function_struct.hpp"

namespace stardec {
    // template<class T>
    // using aggregationfunction = typename std::function<T(const std::unordered_map<unsigned int, T>&, const std::unordered_map<unsigned int, double>&)>;

    template<typename... Value>
    class tree {
    public:
        tree() : _root(std::make_unique<node<Value...>>("")) {};
        leafnode<Value...>* root() {return _root.get();}

        void compute_optimal_policy(const decision_function<Value...> &fct) {
            fct(_root.get());
        }

        void compute_optimal_policy(const std::vector<decision_function<Value...>> &fcts) {
            _root.profile = std::vector<double>(fcts.size(), 1.0/fcts.size());
        }

        std::string to_dot() const {
            unsigned int id = 0;
            std::stringstream s;
            s << "graph tree {" << std::endl;
            s << "rankdir=LR" << std::endl;
            s << "root [shape=square]" << std::endl;
            for(auto &&c : _root->children)
                build_children(s, "root", c.get(), false, id);
            s << "}" << std::endl;
            return s.str();
        }

        void build_children(std::stringstream &s, const std::string &parent, const leafnode<Value...> * const node, bool square, unsigned int &id) const {
            std::string p = "node" + std::to_string(++id), shape, edge;
            s << p;
            edge  = " [label=\"" + node->label + "\"]";
            if(node->is_leaf())
                shape = "none, label=\"" + node->str() + "\"]";
            else
                shape = (square ? "square]" : "circle]");
            s << " [shape=" << shape << std::endl;
            s << parent << " -- " << p << edge << std::endl;

            for(auto &&c : node->children)
                build_children(s, p, c.get(), !square, id);
        }

    private:
        std::unique_ptr<node<Value...>> _root;
    };
}

#endif //STARDEC_TREE_H
