#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

#include <memory>
#include <vector>

namespace stardec {
    template <class T>
    class leafnode {
    public:
        leafnode(T v, std::string l) : value(v), label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::shared_ptr<leafnode<T>> node) { throw std::invalid_argument("Can't add a child to a leaf."); }

        T value;
        std::string label;
        std::vector<std::shared_ptr<leafnode<T>>> children;
        std::shared_ptr<leafnode<T>> optimal;
        std::vector<double> model;
    };

    template<class T>
    class node : public leafnode<T> {
    public:
        node(std::string l);
        bool is_leaf() const {return false;}
        void add_child(std::shared_ptr<leafnode<T>> node) { this->children.push_back(node); }
    };    
}

#endif //STARDEC_NODE_H
