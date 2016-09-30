#ifndef STARDEC_LOGICALOPERATOR_H
#define STARDEC_LOGICALOPERATOR_H

#include <string>
#include <iostream>

namespace stardec {
    enum oper {AND_OP=1, OR_OP=2, NOT_OP=3, ARG_OP=4};

    class LogicalOperator {
    public:
        static LogicalOperator *BuildArg(std::string a) {
            return new LogicalOperator(oper::ARG_OP, nullptr, nullptr, a);
        }

        static LogicalOperator *BuildNot(LogicalOperator *log) {
            return new LogicalOperator(oper::NOT_OP, log, nullptr, "");
        }

        static LogicalOperator *BuildOr(LogicalOperator *left, LogicalOperator *right) {
            return new LogicalOperator(oper::OR_OP, left, right, "");
        }

        static LogicalOperator *BuildAnd(LogicalOperator *left, LogicalOperator *right) {
            return new LogicalOperator(oper::AND_OP, left, right, "");
        }

        ~LogicalOperator() {
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
        LogicalOperator *left;
        LogicalOperator *right;
        std::string arg;
    private:
        LogicalOperator(oper t, LogicalOperator *l, LogicalOperator *r, std::string a) :
                type(t), left(l), right(r), arg(a) {}
    };
}

#endif //STARDEC_LOGICALOPERATOR_H
