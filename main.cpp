#include <iostream>
#include "graph.h"
#include "tree.h"
#include "evaluator.h"
#include "filter_functions.h"
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

int main(int argc, char *argv[]) {
    srand(time(NULL));
    bool optimize = false, mtbuild = false, verbose = false;
    unsigned int horizon = 5;
    std::string inputfilename;

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
    options.add_options("Execution filters")
            ("goalatks", "Allows the proponent to attack its own goals")
            ("relevant", "Removes executions with irrelevant arguments (arguments not connected to the active graph that are not in a new connected component)")
            ;
    options.add_options("Optimization")
            ("optimize", "Enables policy computation", cxxopts::value<bool>(optimize))
            ;

    options.parse(argc, argv);
    if(options.count("help")) {
        std::cout << options.help({"", "Decision tree building", "Execution filters", "Optimization"}) << std::endl;
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

    //std::cout << g.distribution().to_str() << std::endl;
    if(verbose)
        std::cout << "Building tree" << std::endl;
    std::vector<stardec::filterfunction> filt({stardec::remove_duplicate, std::bind(stardec::relevant, std::cref(g), std::placeholders::_1, std::placeholders::_2)});
    auto update = stardec::fast_ambivalent;
    auto valuation = stardec::present;
    auto agg = stardec::average;
    stardec::tree t(g, filt, update, valuation, agg, horizon, verbose, mtbuild, true);

    if(verbose)
        std::cout << "Computing policy" << std::endl;
    auto dec = std::bind(stardec::maximax, std::placeholders::_1, 0.9);
    t.optimize(dec);
    stardec::evaluator eval;
    if(verbose)
        std::cout << "Evaluating policy" << std::endl;
        
    auto prop = stardec::uniform, opp = stardec::uniform;
    auto p = eval.evaluate(t.root(), prop, opp);
    //std::cout << t.to_dot() << std::endl;
    //for(auto s : p.first)
      //std::cout << s << " ";
    if(verbose)
        std::cout << "Result: ";
    for(auto v : p.second)
        std::cout << v << " ";
    std::cout << std::endl;
    return 0;
}
