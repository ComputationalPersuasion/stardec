#ifndef STARDEC_DECISION_FUNCTIONS_H
#define STARDEC_DECISION_FUNCTIONS_H

#include <memory>
#include <algorithm>
#include <iterator>
#include <string>
#include "node.h"

namespace stardec {
    bool comp(const std::shared_ptr<leafnode> &a, const std::shared_ptr<leafnode> &b) {
        return a->values[0] < b->values[0];
    }

    std::vector<std::shared_ptr<leafnode>>::const_iterator max(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = std::max_element(node->children.cbegin(), node->children.cend(), comp);
        auto maxnode = *it;
        node->optimal = maxnode;
        node->values[0] = maxnode->values[0] * discount_factor;
        return it;
    }

    void maxchance(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = max(node, discount_factor);
        node->model = std::vector<double>(node->children.size(), 0.0);
        unsigned int index = std::distance(node->children.cbegin(), it);
        node->model[index] = 1.0;
    }

    std::vector<std::shared_ptr<leafnode>>::const_iterator min(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = std::min_element(node->children.cbegin(), node->children.cend(), comp);
        auto minnode = *it;
        node->optimal = minnode;
        node->values[0] = minnode->values[0] * discount_factor;
        return it;
    }

    void minchance(std::shared_ptr<leafnode> node, double discount_factor) {
        auto it = min(node, discount_factor);
        node->model = std::vector<double>(node->children.size(), 0.0);
        unsigned int index = std::distance(node->children.cbegin(), it);
        node->model[index] = 1.0;
    }

    void maximax(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children) {
                if(!leaves->is_leaf())
                    maximax(leaves, discount_factor);
            }
            if(!child->is_leaf()) {
                maxchance(child, discount_factor);

            }
        }
        max(node, discount_factor);
    }

    void maximin(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children) {
                if(!leaves->is_leaf())
                    maximin(leaves, discount_factor);
            }
            if(!child->is_leaf()) {
                minchance(child, discount_factor);
            }
        }
        max(node, discount_factor);
    }

    void laplace(std::shared_ptr<leafnode> node, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children) {
                if(!leaves->is_leaf())
                    laplace(leaves, discount_factor);
            }
            if(!child->is_leaf()) {
                unsigned int size = child->children.size();
                std::vector<double> values(size);
                std::transform(child->children.cbegin(), child->children.cend(), values.begin(),
                               [size](auto node) {return node->values[0] / size;});
                child->values[0] = std::accumulate(values.begin(), values.end(), 0.0);
                child->model = std::vector<double>(size, 1.0 / size);
            }
        }
        max(node, discount_factor);
    }

    void hurwicz(std::shared_ptr<leafnode> node, double alpha, double discount_factor) {
        for(auto child : node->children) {
            for(auto leaves : child->children) {
                if(!leaves->is_leaf())
                    hurwicz(leaves, alpha, discount_factor);
            }
            if(!child->is_leaf()) {
                auto beg = child->children.cbegin();
                auto it = std::minmax_element(beg, child->children.cend(), comp);
                auto minnode = *(it.first), maxnode = *(it.second);
                child->values[0] = discount_factor * (alpha * maxnode->values[0] + (1 - alpha) * minnode->values[0]);
                child->model = std::vector<double>(child->children.size(), 0.0);
                unsigned int indexmin = std::distance(beg, it.first), indexmax = std::distance(beg, it.second);
                child->model[indexmax] = alpha;
                child->model[indexmin] += (1 - alpha); //+= in case min == max => probability of 1
            }
        }
        max(node, discount_factor);
    }
}

#endif //STARDEC_DECISION_FUNCTIONS_H
