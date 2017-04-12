#ifndef STARDEC_FUNCTION_ALIAS_H
#define STARDEC_FUNCTION_ALIAS_H

#include <functional>
#include <set>
#include <vector>
#include <unordered_map>
#include "argument.hpp"
#include "node.hpp"
#include "values.hpp"
#include "sequence.hpp"

namespace stardec {
    template <typename... Value> using filter_function = std::function<void(std::set<argument<Value...>*> &, const sequence<Value...> &)>;
    template <typename... Value> using belief_function = std::function<belief(argument<Value...>*)>;
    template <typename... Value> using decision_function = std::function<void(leafnode<Value...>*)>;
    //template <typename... Value> using filter_function = std::function<bool(const argument<Value...>* const, const std::vector<argument<Value...>*>&)>;
    //using updatefunction = std::function<void(splittercell::distribution&, const argument&)>;
    //template<typename T>
    //using valuationfunction = typename std::function<std::unordered_map<unsigned int, T>(const graph&, const std::vector<std::string>&)>;
    //template<typename T>
    //using aggregationfunction = typename std::function<T(const std::unordered_map<unsigned int, T>&, const std::unordered_map<unsigned int, double>&)>;
}

#endif
