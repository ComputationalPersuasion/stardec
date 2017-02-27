#ifndef STARDEC_FUNCTION_STRUCT_H
#define STARDEC_FUNCTION_STRUCT_H

#include "function_alias.hpp"

namespace stardec {
    template <typename... Values>
    struct function_struct {
        function_struct(const function_struct &other) : filters(other.filters), update(other.update), valuation(other.valuation) {};

        std::vector<filterfunction>              filters;
        updatefunction                           update;
        std::tuple<valuationfunction<Values>...> valuation;
    };
}

#endif
