#ifndef STARDEC_PARSER_STRUCT_H
#define STARDEC_PARSER_STRUCT_H

#include <string>
#include <vector>
#include <memory>
#include "graph.hpp"

namespace stardec {
    struct parsed_argument {
        parsed_argument(const std::string &lbl) : label(lbl) {}

        std::string label;
    };

    struct parsed_words {
        parsed_words(const std::string &lbl, const std::vector<std::string> &w) : label(lbl), words(w) {}

        std::string label;
        std::vector<std::string> words;
    };

    struct parsed_attack {
        parsed_attack(const std::string &f, const std::string &t) : from(f), to(t) {}

        std::string from, to;
    };

    struct parsed_goal {
        parsed_goal(const std::string &g) : label(g) {}

        std::string label;
    };

    struct parsed_item {
        enum {ARG, WORDS, ATK, GOAL} type;
        std::unique_ptr<parsed_argument> arg;
        std::unique_ptr<parsed_words> words;
        std::unique_ptr<parsed_attack> atk;
        std::unique_ptr<parsed_goal> goal;
    };

    template <typename... Value>
    void use(parsed_argument *arg, graph<Value...> &g) {
        g.add_argument(std::make_shared<argument<Value...>>(arg->label, g.number_of_arguments() + 1));
    }

    template <typename... Value>
    void use(parsed_words *w, graph<Value...> &g) {
        g.arg(w->label)->add_words(w->words);
    }

    template <typename... Value>
    void use(parsed_attack *atk, graph<Value...> &g) {
        g.attack(atk->from, atk->to);
    }

    template <typename... Value>
    void use(parsed_goal *goal, graph<Value...> &g) {
        g.set_goal(goal->label);
    }
}

void parse(const std::string &, stardec::parsed_item &, bool);

namespace stardec {
    class graphbuilder {
    public:
        template <typename... Value>
        static void build_graph_from_file(graph<Value...> &g, std::string filename, bool verbose=false) {
            std::ifstream f(filename);
            std::string line;
            parsed_item item;
            while(std::getline(f, line)) {
                if(line.empty()) continue;
                parse(line, item, verbose);
                switch(item.type) {
                    case parsed_item::ARG: use(item.arg.get(), g); break;
                    case parsed_item::WORDS: use(item.words.get(), g); break;
                    case parsed_item::GOAL: use(item.goal.get(), g); break;
                    case parsed_item::ATK: use(item.atk.get(), g); break;
                    default: throw std::domain_error("Undefined parsed type");
                }
            }
            f.close();
        }
    };
}

#endif //STARDEC_PARSER_STRUCT_H
