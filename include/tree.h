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

namespace stardec {
    typedef std::function<void(std::set<std::string>&, const std::vector<std::string>&)> filterfunction;
    typedef std::function<void(splittercell::distribution&, const argument&)> updatefunction;
    template<class T>
    using valuationfunction = typename std::function<std::unordered_map<unsigned int, T>(const graph&, const std::vector<std::string>&)>;
    template<class T>
    using aggregationfunction = typename std::function<T(const std::unordered_map<unsigned int, T>&, const std::unordered_map<unsigned int, double>&)>;
    template<class T> using decisionfunction = typename std::function<void(std::shared_ptr<leafnode<T>>)>;

    template<class T>
    class tree {
    public:
        tree(const graph &g, const std::vector<filterfunction> &filter_functions,
            const updatefunction &update_function, const valuationfunction<T> &valuation_function,
            const aggregationfunction<T> &aggregation_function, unsigned int horizon, bool verbose, bool mtbuild = true, bool mtdist = true);
        std::string to_dot() const;
        leafnode<T>* root() {return _root.get();}

        void optimize(const decisionfunction<T> &decision_function);

    private:
        std::shared_ptr<node<T>> _root;
    };
}


#endif //STARDEC_TREE_H
