#ifndef STARDEC_NODE_TEMPLATE_H
#define STARDEC_NODE_TEMPLATE_H

#include <memory>
#include <vector>
#include <tuple>

namespace stardec {
    template <typename... Values>
    leafnode<Values...>::leafnode(Values... v, const std::string &l) : value(std::make_tuple(v...)), label(l), optimal(nullptr) {}

    template <typename... Values>
    node<Values...>::node(const std::string &l) : leafnode<Values...>(l) {}
}

#endif //STARDEC_NODE_TEMPLATE_H
