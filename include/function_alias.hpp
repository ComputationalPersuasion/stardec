#ifndef STARDEC_FUNCTION_ALIAS_H
#define STARDEC_FUNCTION_ALIAS_H

#include <functional>
#include <set>
#include <vector>
#include <unordered_map>
#include "argument.hpp"
#include "values.hpp"

namespace stardec {
    //using filterfunction = std::function<void(std::set<std::string>&, const std::vector<std::string>&)>;
    template <typename... Values> using belief_function = std::function<belief(argument<Values...>*)>;
    //using filter_function = std::function<bool(const argument* const, const std::vector<argument*>&)>;
    //using updatefunction = std::function<void(splittercell::distribution&, const argument&)>;
    //template<typename T>
    //using valuationfunction = typename std::function<std::unordered_map<unsigned int, T>(const graph&, const std::vector<std::string>&)>;
    //template<typename T>
    //using aggregationfunction = typename std::function<T(const std::unordered_map<unsigned int, T>&, const std::unordered_map<unsigned int, double>&)>;
}

#endif
