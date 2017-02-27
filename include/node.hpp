#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

#include <memory>
#include <vector>
#include <tuple>

namespace stardec {
    template <typename... Values>
    class leafnode {
    public:
        leafnode(const std::string &l);
        leafnode(Values... v, const std::string &l);
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::unique_ptr<leafnode<Values...>> node) { throw std::invalid_argument("Can't add a child to a leaf."); }

        std::tuple<Values...> value;
        std::string label;
        std::vector<std::unique_ptr<leafnode<Values...>>> children;
        leafnode<Values...> *optimal;
        std::vector<double> model;
    };

    template<typename... Values>
    class node : public leafnode<Values...> {
    public:
        node(const std::string &l);
        bool is_leaf() const override {return false;}
        void add_child(std::unique_ptr<leafnode<Values...>> node) override { this->children.push_back(node); }
    };
}

#include "templates/node.tpp"

#endif //STARDEC_NODE_H
