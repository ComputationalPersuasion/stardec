#include <iostream>
#include <unordered_map>
#include <fstream>
#include "graph.h"
#include "tree.h"
#include "evaluator.h"
#include "filter_functions.h"
#include "distance.h"
#include "decision_functions.h"
#include "valuation_functions.h"
#include "update_functions.h"
#include "aggregation_functions.h"
#include "evaluation_functions.h"
#include "cxxopts.hpp"
extern "C" {
    #include "parser.hpp"
}
#include <csignal>

extern FILE *yyin;

void stop(int s) {
  exit(0);
}

void load_circumplex_values(std::unordered_map<std::string, std::unordered_map<std::string, std::array<double, 3>>> &values) {
    unsigned int nbline;
    double valence, arousal, dominance;
    std::string group, word;
    std::ifstream f;

    f.open("resources/values.csv", std::fstream::in);
    f >> nbline;
    for(unsigned int g = 0; g < 7; g++) {
        f >> group;
        for(unsigned int i = 0; i < nbline; i++) {
            f >> word >> valence >> arousal >> dominance;
            values[group][word] = {valence, arousal, dominance};
        }
    }
    f.close();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    bool optimize = false, mtbuild = false, verbose = false;
    unsigned int horizon = 5;
    std::string inputfilename, category = "all";

    cxxopts::Options options("stardec", "STrategic ARgumentation using DECision trees");
    options.add_options()
            ("f,file", "ASPARTIX extended argumentation problem file", cxxopts::value<std::string>(inputfilename), "FILENAME")
            ("h,help", "Displays help screen")
            ("v,verbose", "Displays information such as parsed items, time, etc.", cxxopts::value<bool>(verbose))
            ;
    options.add_options("Decision tree building")
            ("buildmt", "Use multi-threading to built the decision tree (default false).", cxxopts::value<bool>(mtbuild))
            ("horizon", "Total number of arguments that can be played in the dialogue. Maximum depth of the decision tree (default 5).",
             cxxopts::value<unsigned int>(horizon), "HORIZON")
            ;
    options.add_options("Emotions")
            ("category", "Category of people to use for the circumplex model of emotion values (default all).", cxxopts::value<std::string>(category), "CATEGORY")
            ;
    options.add_options("Execution filters")
            ("goalatks", "Allows the proponent to attack its own goals")
            ("relevant", "Removes executions with irrelevant arguments (arguments not connected to the active graph that are not in a new connected component)")
            ;
    options.add_options("Optimization")
            ("optimize", "Enables policy computation", cxxopts::value<bool>(optimize))
            ;

    options.parse(argc, argv);
    if(options.count("help")) {
        std::cout << options.help({"", "Decision tree building", "Execution filters", "Optimization", "Emotions"}) << std::endl;
        return 0;
    }
    if(!options.count("file")) {
        std::cout << "No input file specified. Use -h for help." << std::endl;
        return -1;
    }

    yyin = fopen(inputfilename.c_str(), "r");
    stardec::graph g;
    yyparse(g, verbose);
    fclose(yyin);

    signal(SIGINT, stop);

    if(verbose)
        std::cout << "Building tree" << std::endl;

    //std::unordered_map<std::string, std::unordered_map<std::string, std::array<double, 3>>> values;
    //load_circumplex_values(values);

    std::vector<stardec::filterfunction> filt({stardec::remove_duplicate, std::bind(stardec::relevant, std::cref(g), std::placeholders::_1, std::placeholders::_2)});
    auto update = stardec::fast_ambivalent;
    auto valuation = stardec::present;
    //auto valuation = std::bind(stardec::circumplex, std::placeholders::_1, std::placeholders::_2, std::cref(values.at(category)));
    auto agg = stardec::average;
    stardec::tree<double> t(g, filt, update, valuation, agg, horizon, verbose, mtbuild, true);

    // std::cout << t.to_dot() << std::endl;
    // return 0;

    if(verbose)
        std::cout << "Computing policy" << std::endl;
    //auto ref = std::bind(stardec::biaised_ideal, std::placeholders::_1, std::vector<double>({1, 0.8, 0.8, 1, 1}));
    //auto dec = std::bind(stardec::rpemomax, std::placeholders::_1, std::cref(ref));
    auto dec = std::bind(stardec::maximin, std::placeholders::_1, 0.9);
    t.optimize(dec);
    stardec::evaluator<double> eval;
    if(verbose)
        std::cout << "Evaluating policy" << std::endl;

    auto prop = stardec::optimal<double>, opp = stardec::optimal<double>;
    auto p = eval.evaluate(t.root(), prop, opp);
    // std::cout << t.to_dot() << std::endl;
    //for(auto s : p.first)
      //std::cout << s << " ";
    if(verbose) {
        std::cout << "Result: ";
        for(auto s : p.first)
            std::cout << s << " ";
    }
    return 0;
}
