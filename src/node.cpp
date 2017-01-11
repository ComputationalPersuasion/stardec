#include "node.h"

namespace stardec {
  template<> node<double>::node(std::string l) : leafnode<double>(0,l) {}
  template<> node<std::vector<double>>::node(std::string l) : leafnode<std::vector<double>>({0},l) {}
}
