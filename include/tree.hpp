#ifndef STARDEC_TREE_H
#define STARDEC_TREE_H

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
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
            
        }

    private:
        std::unique_ptr<node<Value...>> _root;
    };
}

#endif //STARDEC_TREE_H
