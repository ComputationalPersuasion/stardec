#ifndef STARDEC_NODE_H
#define STARDEC_NODE_H

#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>

namespace stardec {
    template <typename... Value>
    class leafnode {
    public:
        leafnode(Value... v, const std::string &l) : value(std::make_tuple(v...)), label(l), optimal(nullptr) {}
        leafnode(const std::string &l) : label(l), optimal(nullptr) {}
        virtual bool is_leaf() const {return true;}
        virtual void add_child(std::unique_ptr<leafnode<Value...>>&&) {throw std::invalid_argument("Can't add a child to a leaf.");}
        virtual bool operator<(const leafnode<Value...> &other) const {return comp<Value...>(other) == -1;}

        std::string str() const {
            std::stringstream s;
            s << "[";
            str<Value...>(s);
            s << "]";
            return s.str();
        }

        std::tuple<Value...> value;
        std::string label;
        std::vector<std::unique_ptr<leafnode<Value...>>> children;
        leafnode<Value...> *optimal;
        std::vector<double> model, profile;

    private:
        template <typename T>
        int comp(const leafnode<Value...> &other) const {
            return std::get<T>(value).comp(std::get<T>(other.value));
        }

        template <typename T, typename U, typename... Other>
        int comp(const leafnode<Value...> &other) const {
            int co = comp<U, Other...>(other);
            if(co == -2) return -2;
            int c0 = std::get<T>(value).comp(std::get<T>(other.value));

            if(co == -c0) return -2;
            int ret = c0 + co;
            if(ret < -1) ret = -1;
            else if(ret > 1) ret = 1;

            return ret;
        }

        template <typename T>
        void str(std::stringstream &s) const {
            s << std::get<T>(value).str();
        }

        template <typename T, typename U, typename... Other>
        void str(std::stringstream &s) const {
            s << std::get<T>(value).str() << ",";
            str<U,Other...>(s);
        }
    };

    template <typename... Value>
    class node : public leafnode<Value...> {
    public:
        node(const std::string &l) : leafnode<Value...>(l) {}
        virtual bool is_leaf() const override {return false;}
        virtual void add_child(std::unique_ptr<leafnode<Value...>> &&node) override {this->children.push_back(std::move(node));}
    };
}

#endif //STARDEC_NODE_H
