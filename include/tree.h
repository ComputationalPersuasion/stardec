#ifndef STARDEC_TREE_H
#define STARDEC_TREE_H

#include <string>
#include <memory>
#include <functional>
#include "graph.h"
#include "filter_functions.h"
#include "decision_functions.h"
#include "node.h"

typedef std::function<void(std::set<std::string>&, const std::vector<std::string>&)> FilterFunction;

namespace stardec {
    class Tree {
    public:
        Tree(const Graph &graph, std::vector<FilterFunction> filter_functions, unsigned int horizon);

    private:
        std::unique_ptr<Node> _root;
    };
}


#endif //STARDEC_TREE_H
