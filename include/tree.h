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
    typedef std::function<std::unordered_map<unsigned int, double>(const graph&, const std::vector<std::string>&)> valuationfunction;
    typedef std::function<double(const std::unordered_map<unsigned int, double> &, const std::unordered_map<unsigned int, double> &)> aggregationfunction;

    class tree {
    public:
        tree(const graph &g, const std::vector<filterfunction> &filter_functions,
          const updatefunction &update_function,
          const valuationfunction &valuation_function,
          const aggregationfunction &aggregation_function,
          unsigned int horizon);

    private:
        std::unique_ptr<node> _root;
    };
}


#endif //STARDEC_TREE_H
