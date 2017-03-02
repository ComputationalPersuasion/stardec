#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

#include <memory>
#include <vector>
#include <tuple>

namespace stardec {
    template <typename... Value>
    class leafnode {
    public:
        leafnode(Value... v, const std::string &l) : value(std::make_tuple(v...)), label(l), optimal(nullptr) {}
        leafnode(const std::string &l) : label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::unique_ptr<leafnode<Value...>>&) { throw std::invalid_argument("Can't add a child to a leaf."); }
        virtual int comp(leafnode<Value...> *other) {

        }

        std::tuple<Value...> value;
        std::string label;
        std::vector<std::unique_ptr<leafnode<Value...>>> children;
        leafnode<Value...> *optimal;
        std::vector<double> model;
    };

    template <typename... Value>
    class node : public leafnode<Value...> {
    public:
        node(const std::string &l) : leafnode<Value...>(l) {}
        bool is_leaf() const override {return false;}
        void add_child(std::unique_ptr<leafnode<Value...>> &&node) override { this->children.push_back(std::move(node)); }
    };
}

#endif //STARDEC_NODE_H
