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
    typedef std::function<std::unordered_map<unsigned int, std::vector<double>>(const graph&, const std::vector<std::string>&)> valuationfunction;
    typedef std::function<std::vector<double>(const std::unordered_map<unsigned int, std::vector<double>> &, const std::unordered_map<unsigned int, double> &)> aggregationfunction;
    typedef std::function<void(std::shared_ptr<leafnode> node)> decisionfunction;

    class tree {
    public:
        tree(const graph &g, const std::vector<filterfunction> &filter_functions,
            const updatefunction &update_function, const valuationfunction &valuation_function,
            const aggregationfunction &aggregation_function, unsigned int horizon, bool verbose, bool mtbuild = true, bool mtdist = true);
        std::string to_dot() const;
        leafnode* root() {return _root.get();}

        void optimize(const decisionfunction &decision_function);

    private:
        std::shared_ptr<node> _root;
    };
}


#endif //STARDEC_TREE_H
