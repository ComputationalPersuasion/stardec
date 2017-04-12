#ifndef STARDEC_DECISION_FUNCTIONS_H
#define STARDEC_DECISION_FUNCTIONS_H

#include <memory>
#include <algorithm>
#include <iterator>
#include <string>
#include "node.hpp"

namespace stardec {
    /*typedef std::function<std::vector<double>(const std::set<std::vector<double>>&)> ref_function;
    typedef std::function<double(const std::vector<double> &)> dist_function;
    template <class T>
    using comp_function = typename std::function<bool(const std::shared_ptr<leafnode<T>>&, const std::shared_ptr<leafnode<T>>&)>;

    bool comp(const std::shared_ptr<leafnode<double>> &a, const std::shared_ptr<leafnode<double>> &b) {
        return a->value < b->value;
    }*/

    /*bool dominated(const std::shared_ptr<leafnode<std::vector<double>>> &a, const std::shared_ptr<leafnode<std::vector<double>>> &b) {
        bool dominated = true;
        if((a->value[3] == 0.0) && (b->value[3] == 1.0))
            return true;
        if((a->value[3] == 1.0) && (b->value[3] == 0.0))
            return false;
        for(unsigned int i = 0; i < a->value.size(); i++)
            dominated &= (a->value[i] <= b->value[i]);
        return dominated;
    }*/

    template <typename... Value>
    typename std::vector<std::unique_ptr<leafnode<Value...>>>::const_iterator max(leafnode<Value...>* node) {
        auto it = std::max_element(node->children.cbegin(), node->children.cend());
        auto maxnode  = (*it).get();
        node->optimal = maxnode;
        node->value   = maxnode->value;
        return it;
    }

    /*void best_compromise(std::shared_ptr<leafnode<std::vector<double>>> node, const dist_function &dist) {
      std::vector<std::shared_ptr<leafnode<std::vector<double>>>> non_dominated;
      std::vector<std::shared_ptr<leafnode<std::vector<double>>>> to_check(node->children);
      for(auto it = to_check.cbegin(); it != to_check.cend(); ++it) {
          bool dom = false;
          for(auto itt = to_check.cbegin(); itt != to_check.cend(); ++itt) {
              if(it == itt)
                  continue;
              dom |= dominated(*it, *itt);
              if(dom) {
                  it = to_check.erase(it);
                  --it;
                  break;
              }
              if(dominated(*itt, *it)) {
                  itt = to_check.erase(itt);
                  --itt;
              }
          }
          if(!dom)
              non_dominated.push_back(*it);
      }
      auto it = std::min_element(non_dominated.cbegin(), non_dominated.cend(), [&dist](auto n1, auto n2) {return dist(n1->value) < dist(n2->value);});
      auto maxnode  = *it;
      node->optimal = maxnode;
      node->value   = maxnode->value;
    }*/

    template <typename... Value>
    void maxchance(leafnode<Value...> *node) {
        auto it = max(node);
        node->model = std::vector<double>(node->children.size(), 0.0);
        unsigned int index = std::distance(node->children.cbegin(), it);
        node->model[index] = 1.0;
    }

    template <typename... Value>
    typename std::vector<std::unique_ptr<leafnode<Value...>>>::const_iterator min(leafnode<Value...> *node) {
        auto it = std::min_element(node->children.cbegin(), node->children.cend());
        auto minnode  = (*it).get();
        node->optimal = minnode;
        node->value   = minnode->value;
        return it;
    }

    // std::vector<std::shared_ptr<leafnode<std::vector<double>>>>::const_iterator min(std::shared_ptr<leafnode<std::vector<double>>> node) {
    //     auto it = std::min_element(node->children.cbegin(), node->children.cend(), dominated);
    //     auto minnode  = *it;
    //     node->optimal = minnode;
    //     node->value   = minnode->value;
    //     return it;
    // }

    template <typename... Value>
    void minchance(leafnode<Value...> *node) {
        auto it = min(node);
        node->model = std::vector<double>(node->children.size(), 0.0);
        unsigned int index = std::distance(node->children.cbegin(), it);
        node->model[index] = 1.0;
    }

    // void minchance(std::shared_ptr<leafnode<std::vector<double>>> node) {
    //     auto it = min(node);
    //     node->model = std::vector<double>(node->children.size(), 0.0);
    //     unsigned int index = std::distance(node->children.cbegin(), it);
    //     node->model[index] = 1.0;
    // }

    template <typename... Value>
    void maximax(leafnode<Value...> *node) {
        for(auto &&child : node->children) {
            for(auto &&leaf : child->children)
                if(!leaf->is_leaf())
                    maximax(leaf.get());
            if(!child->is_leaf())
                maxchance(child.get());
        }
        max(node);
    }

    template <typename... Value>
    void maximin(leafnode<Value...> *node) {
        for(auto &&child : node->children) {
            for(auto &&leaves : child->children)
                if(!leaves->is_leaf())
                    maximin(leaves.get());
            if(!child->is_leaf())
                minchance(child.get());
        }
        max(node);
    }

    /*void laplace(std::shared_ptr<leafnode<double>> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children)
                if(!leaves->is_leaf())
                    laplace(leaves, discount_factor);
            if(!child->is_leaf()) {
                unsigned int size = child->children.size();
                std::vector<double> values(size);
                std::transform(child->children.cbegin(), child->children.cend(), values.begin(),
                               [size](auto node) {return node->value / size;});
                child->value = std::accumulate(values.begin(), values.end(), 0.0);
                child->model = std::vector<double>(size, 1.0 / size);
            }
        }
        max(node, discount_factor);
    }

    void hurwicz(std::shared_ptr<leafnode<double>> node, double alpha, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children)
                if(!leaves->is_leaf())
                    hurwicz(leaves, alpha, discount_factor);
            if(!child->is_leaf()) {
                auto beg = child->children.cbegin();
                auto it = std::minmax_element(beg, child->children.cend(), comp);
                auto minnode = *(it.first), maxnode = *(it.second);
                child->value = discount_factor * (alpha * maxnode->value + (1 - alpha) * minnode->value);
                child->model = std::vector<double>(child->children.size(), 0.0);
                unsigned int indexmin = std::distance(beg, it.first), indexmax = std::distance(beg, it.second);
                child->model[indexmax] = alpha;
                child->model[indexmin] += (1 - alpha); //+= in case min == max => probability of 1
            }
        }
        max(node, discount_factor);
    }*/

    /*void rpemomax(std::shared_ptr<leafnode<std::vector<double>>> node, const ref_function &ref) {
        auto distance = std::bind(stardec::euclidean, std::placeholders::_1, std::placeholders::_2, std::vector<double>({1.0, 1.0, 1.0, 0, 0}), std::vector<double>({9.0, 9.0, 9.0, 1.0, 1.0}));
        for(auto child : node->children) {
            for(auto leaves : child->children)
                if(!leaves->is_leaf())
                    rpemomax(leaves, ref);
            if(!child->is_leaf()) {
                std::set<std::vector<double>> points;
                std::transform(child->children.cbegin(), child->children.cend(), std::inserter(points, points.begin()), [](auto c){return c->value;});
                auto ref_point = ref(points);
                best_compromise(child, std::bind(distance, std::placeholders::_1, std::cref(ref_point)));
            }
        }
        std::set<std::vector<double>> points;
        std::transform(node->children.cbegin(), node->children.cend(), std::inserter(points, points.begin()), [](auto c){return c->value;});
        auto ref_point = ref(points);
        best_compromise(node, std::bind(distance, std::placeholders::_1, std::cref(ref_point)));
    }*/

    // void rpemomin(std::shared_ptr<leafnode<std::vector<double>>> node, const ref_function &ref) {
    //     for(auto child : node->children) {
    //         for(auto leaves : child->children)
    //             if(!leaves->is_leaf())
    //                 rpemomin(node, ref);
    //         if(!child->is_leaf())
    //             minchance(child);
    //     }
    //     max(node);
    // }
}

#endif //STARDEC_DECISION_FUNCTIONS_H
