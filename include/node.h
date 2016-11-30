#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

#include <memory>
#include <vector>

namespace stardec {
    class leafnode {
    public:
        leafnode(std::vector<double> v, std::string l) : values(v), label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::shared_ptr<leafnode> node) { throw std::invalid_argument("Can't add a child to a leaf."); }

        std::vector<double> values;
        std::string label;
        std::vector<std::shared_ptr<leafnode>> children;
        std::shared_ptr<leafnode> optimal;
        std::vector<double> model;
    };

    class node : public leafnode {
    public:
        node(std::string l) : leafnode({0}, l) {}
        bool is_leaf() const {return false;}
        void add_child(std::shared_ptr<leafnode> node) { children.push_back(node); }
    };
}

#endif //STARDEC_NODE_H
