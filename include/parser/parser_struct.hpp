#ifndef STARDEC_PARSER_STRUCT_H
#define STARDEC_PARSER_STRUCT_H

#include <string>
#include <vector>
#include <memory>

namespace stardec {
    enum parsed_type {ARG, WORDS, ATK, GOAL};

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
        parsed_type type;
        std::unique_ptr<parsed_argument> arg;
        std::unique_ptr<parsed_words> words;
        std::unique_ptr<parsed_attack> atk;
        std::unique_ptr<parsed_goal> goal;
    };
}

#endif //STARDEC_PARSER_STRUCT_H
