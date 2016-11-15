#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

namespace stardec {
    class leafnode {
    public:
        leafnode(double v, std::string l) : value(v), label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::shared_ptr<leafnode> node) { throw std::invalid_argument("Can't add a child to a leaf."); }
        virtual bool operator<(const leafnode &other) const {return value < other.value;}

        double value;
        std::string label;
        std::unordered_map<std::string, std::shared_ptr<leafnode>> children;
        std::shared_ptr<leafnode> optimal;
    };

    class node : public leafnode {
    public:
        node(std::string l) : leafnode(0, l) {}
        bool is_leaf() const {return false;}
        void add_child(std::shared_ptr<leafnode> node) { children[node->label] = node; }
    };
}

#endif //STARDEC_NODE_H
