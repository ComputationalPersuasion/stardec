#ifndef STARDEC_LOGICALOPERATOR_H
#define STARDEC_LOGICALOPERATOR_H

#include <string>
#include <iostream>

namespace stardec {
    enum oper {AND_OP=1, OR_OP=2, NOT_OP=3, ARG_OP=4};

    class logicaloperator {
    public:
        static logicaloperator *build_arg(std::string a) {
            return new logicaloperator(oper::ARG_OP, nullptr, nullptr, a);
        }

        static logicaloperator *build_not(logicaloperator *log) {
            return new logicaloperator(oper::NOT_OP, log, nullptr, "");
        }

        static logicaloperator *build_or(logicaloperator *left, logicaloperator *right) {
            return new logicaloperator(oper::OR_OP, left, right, "");
        }

        static logicaloperator *build_and(logicaloperator *left, logicaloperator *right) {
            return new logicaloperator(oper::AND_OP, left, right, "");
        }

        ~logicaloperator() {
            if(left != nullptr)
                delete left;
            if(right != nullptr)
                delete right;
        }

        std::string to_s() const {
            switch(type) {
                case ARG_OP: return arg;
                case AND_OP: return "(" + left->to_s() + " && " + right->to_s() + ")";
                case OR_OP: return "(" + left->to_s() + " || " + right->to_s() + ")";
                case NOT_OP: return "!" + left->to_s();
                default: return "";
            }
        }

        oper type;
        logicaloperator *left;
        logicaloperator *right;
        std::string arg;
    private:
        logicaloperator(oper t, logicaloperator *l, logicaloperator *r, std::string a) :
                type(t), left(l), right(r), arg(a) {}
    };
}

#endif //STARDEC_LOGICALOPERATOR_H
