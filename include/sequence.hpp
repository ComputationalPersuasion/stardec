#ifndef STARDEC_SEQUENCE_H
#define STARDEC_SEQUENCE_H

#include <vector>
#include "argument.hpp"

namespace stardec {
    template <typename... Value>
    class sequence {
    public:
        sequence() {}
        sequence(const sequence& other) : _sequence(other._sequence) {}

        unsigned int size() const {return _sequence.size();}
        auto begin() const {return _sequence.begin();}
        auto end() const {return _sequence.end();}
        bool empty() const {return _sequence.empty();}

        void add(argument<Value...>* arg) {_sequence.push_back(arg);}

    private:
        std::vector<argument<Value...>*> _sequence;
    };
}

#endif //STARDEC_SEQUENCE_H
