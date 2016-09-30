#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

namespace stardec {
    class LeafNode {
    public:
        LeafNode(double v, std::string l) : value(v), label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::shared_ptr<LeafNode> node) { throw std::invalid_argument("Can't add a child to a leaf."); }
        virtual bool operator<(const LeafNode &other) const {return value < other.value;}

        double value;
        std::string label;
        std::unordered_map<std::string, std::shared_ptr<LeafNode>> children;
        std::shared_ptr<LeafNode> optimal;
    };

    class Node : public LeafNode {
    public:
        Node(std::string l) : LeafNode(0, l) {}
        bool is_leaf() const {return false;}
        void add_child(std::shared_ptr<LeafNode> node) { children[node->label] = node; }
    };
}

#endif //STARDEC_NODE_H
