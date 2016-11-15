#ifndef STARDEC_DECISION_FUNCTIONS_H
#define STARDEC_DECISION_FUNCTIONS_H

#include <memory>
#include <algorithm>
#include <iterator>
#include <string>
#include "node.h"

namespace stardec {
    void max(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = std::max_element(node->children.begin(), node->children.end(), [](auto a, auto b){return a.second->value < b.second->value;});
        node->optimal = it->second;
        node->value = it->second->value * discount_factor;
    }

    void min(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = std::min_element(node->children.begin(), node->children.end(), [](auto a, auto b){return a.second->value < b.second->value;});
        node->optimal = it->second;
        node->value = it->second->value * discount_factor;
    }

    void maximax(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child.second->children) {
                if(!leaves.second->is_leaf())
                    maximax(leaves.second, discount_factor);
            }
            max(child.second, discount_factor);
        }
        max(node, discount_factor);
    }

    void maximin(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child.second->children) {
                if(!leaves.second->is_leaf())
                    maximin(leaves.second, discount_factor);
            }
            min(child.second, discount_factor);
        }
        max(node, discount_factor);
    }

    void laplace(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child.second->children) {
                if(!leaves.second->is_leaf())
                    laplace(leaves.second, discount_factor);
            }
            unsigned int size = (unsigned int)child.second->children.size();
            std::vector<double> values;
            std::transform(child.second->children.begin(), child.second->children.end(), std::back_inserter(values),
                           [size](auto node) {return node.second->value / size;});
            child.second->value = std::accumulate(values.begin(), values.end(), 0.0);
        }
        max(node, discount_factor);
    }

    void hurwicz(std::shared_ptr<leafnode> node, double alpha, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child.second->children) {
                if(!leaves.second->is_leaf())
                    hurwicz(leaves.second, alpha, discount_factor);
            }
            auto it = std::minmax_element(child.second->children.begin(), child.second->children.end());
            auto minnode = it.first->second, maxnode = it.second->second;
            child.second->value = discount_factor * (alpha * maxnode->value + (1 - alpha) * minnode->value);
        }
        max(node, discount_factor);
    }
}

#endif //STARDEC_DECISION_FUNCTIONS_H
