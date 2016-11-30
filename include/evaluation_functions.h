#ifndef STARDEC_EVALUATION_FUNCTIONS_H
#define STARDEC_EVALUATION_FUNCTIONS_H

#include <algorithm>
#include <cmath>
#include <vector>
#include <random>
#include "node.h"
#include "distance.h"

namespace stardec {
    leafnode* uniform(const leafnode * const node) {
        unsigned int num_of_children = node->children.size();
        unsigned int index = rand() % num_of_children;
        return node->children[index].get();
    }

    leafnode* optimal(const leafnode * const node) {
        return node->optimal.get();
    }

    void generate_distribution(std::vector<double> &profile, std::uniform_int_distribution<> &u_i_d, std::random_device &rd) {
      unsigned int sum = 0;
      for(unsigned int i = 0; i < profile.size(); i++) {
          unsigned int p = u_i_d(rd);
          profile[i] = p;
          sum += p;
      }
      std::transform(profile.cbegin(), profile.cend(), profile.begin(), [sum](double p){return p / sum;});
    }

    leafnode* discrete(const leafnode * const node, double lowbound, double step, std::uniform_int_distribution<> &u_i_d, std::random_device &rd) {
        std::vector<double> profile(node->model.size());
        double distance;
        do {
            generate_distribution(profile, u_i_d, rd);
            distance = jensen_shannon(node->model, profile);
        } while(distance < lowbound || distance > (lowbound + step));
        std::discrete_distribution<> d(profile.cbegin(), profile.cend());
        return node->children[d(rd)].get();
    }
}

#endif
