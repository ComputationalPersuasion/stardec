#ifndef STARDEC_TREE_H
#define STARDEC_TREE_H

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include "graph.h"
#include "node.h"
#include "argument.h"
#include "function_alias.h"
#include "function_struct.h"

namespace stardec {
    // template<class T>
    // using aggregationfunction = typename std::function<T(const std::unordered_map<unsigned int, T>&, const std::unordered_map<unsigned int, double>&)>;
    template<class T> using decisionfunction = typename std::function<void(std::shared_ptr<leafnode<T>>)>;

    template<typename... Values>
    class tree {
    public:
        tree() : _root(std::make_unique<node<Values...>>("")) {};
        tree(std::shared_ptr<function_struct<Belief, Values...>> f_struct, unsigned int horizon, bool mtbuild = true, bool mtdist = true);
        std::string to_dot() const;
        leafnode<Values...>* root() {return _root.get();}

        //void compute_optimal_policy(const decisionfunction<T> &decision_function);

    private:
        std::unique_ptr<node<Values...>> _root;
        bool _mtbuild, _mtdist;
        std::shared_ptr<function_struct> _f_struct;
        unsigned int _horizon;
    };
}

#include "templates/tree.tpp"

#endif //STARDEC_TREE_H
