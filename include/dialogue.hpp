#ifndef STARDEC_DIALOGUE_H
#define STARDEC_DIALOGUE_H

#include <vector>
#include "function_alias.hpp"

namespace stardec {
    class dialogue {
    public:
        dialogue(std::vector<argument*> sequence) : _sequence(sequence) {}

        argument* move_at(unsigned int index) const {return _sequence[index];}
        argument* last_move() const {return _sequence.back();}
        unsigned int length() const {return _sequence.size();}
        
    private:
        std::vector<argument*> _sequence;
    };
}

#endif //STARDEC_DIALOGUE_H
