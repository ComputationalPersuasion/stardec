#ifndef STARDEC_UPDATER_H
#define STARDEC_UPDATER_H

#include <typeinfo>
#include <unordered_map>

namespace stardec {
    class updater {
    public:
        void register_function(std::type_index type, update_function fct) {
            update_functions[type] = fct;
        }

    private:
        std::unordered_map<std::type_index, update_function> update_functions;
    };
}

#endif //STARDEC_UPDATER_H