#include <iostream>
#include "graph.h"
#include "cxxopts.hpp"
extern "C" {
    #include "parser.hpp"
}

extern FILE *yyin;

int main(int argc, char *argv[]) {
    bool optimize = false;
    unsigned int horizon = 5;
    std::string inputfilename;

    cxxopts::Options options("stardec", "STrategic ARgumentation using DECision trees");
    options.add_options()
            ("f,file", "ASPARTIX extended argumentation problem file", cxxopts::value<std::string>(inputfilename), "FILENAME")
            ("h,help", "Displays help screen");
    options.add_options("Decision tree building")
            ("horizon", "Total number of arguments that can be played in the dialogue. Maximum depth of the decision tree (default 5)",
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
    stardec::graph *g = new stardec::graph();
    yyparse(g);
    fclose(yyin);

    std::cout << g->distribution().to_str() << std::endl;

    delete g;
    return 0;
}
