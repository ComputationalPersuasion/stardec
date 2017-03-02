#include <iostream>
#include <unordered_map>
#include <fstream>
//#include "function_alias.hpp"
#include "graph.hpp"
#include "treebuilder.hpp"
//#include "tree.hpp"
// #include "evaluator.hpp"
#include "filter_functions.hpp"
// #include "distance.hpp"
// #include "decision_functions.hpp"
//#include "valuation_functions.hpp"
#include "update_functions.hpp"
//#include "aggregation_functions.hpp"
// #include "evaluation_functions.hpp"
//#include "function_struct.hpp"
#include "cxxopts.hpp"
#include "values.hpp"
#include "parser/parser_helper.hpp"

#define ARG_TYPE stardec::belief, stardec::affective_norm

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
    bool mtbuild = false, verbose = false;
    unsigned int horizon = 5;
    std::string inputfilename, category, update;
    //auto f_struct = std::make_shared<stardec::function_struct<stardec::belief, stardec::valuation, stardec::affective_norm>>;

    cxxopts::Options options("stardec", "STrategic ARgumentation using DECision trees");
    options.add_options()
            ("f,file", "ASPARTIX extended argumentation problem file", cxxopts::value<std::string>(inputfilename), "FILENAME")
            ("h,help", "Displays help screen")
            ("v,verbose", "Displays information such as parsed items, time, etc. (default false)", cxxopts::value<bool>(verbose))
            ;
    options.add_options("Decision tree building")
            ("buildmt", "Use multi-threading to built the decision tree. (default false)", cxxopts::value<bool>(mtbuild))
            ("horizon", "Total number of arguments that can be played in the dialogue. Maximum depth of the decision tree. (default 5)",
             cxxopts::value<unsigned int>(horizon), "HORIZON")
            ;
    options.add_options("Emotions")
            ("category", "Category of people to use for the circumplex model of emotion values.", cxxopts::value<std::string>(category)->default_value("all"), "CATEGORY")
            ;
    options.add_options("Execution filters")
            ("goalatks", "Allows the proponent to attack its own goals.")
            ("allowirrelevant", "Allow irrelevant arguments (arguments not connected to the active graph that are not in a new connected component).")
            ("allowduplicate", "Allow arguments to appear several times in the sequences.")
            ;
    options.add_options("Update Method")
            ("update", "Specify the update method to use.", cxxopts::value<std::string>(update)->default_value("fast_ambivalent"), "METHOD")
            ;

    options.parse(argc, argv);
    if(options.count("help")) {
        std::cout << options.help({"", "Decision tree building", "Update Method", "Execution filters", "Emotions"}) << std::endl;
        return 0;
    }
    if(!options.count("file")) {
        std::cout << "No input file specified. Use -h for help." << std::endl;
        return -1;
    }

    std::unordered_map<std::string, std::unordered_map<std::string, std::array<double, 3>>> values;
    load_circumplex_values(values);

    stardec::graph<ARG_TYPE> g;
    stardec::graphbuilder::build_graph_from_file(g, inputfilename, verbose);
    stardec::tree<ARG_TYPE> t;
    std::vector<stardec::filter_function<ARG_TYPE>> filters({stardec::remove_duplicate<ARG_TYPE>, stardec::relevant<ARG_TYPE>});
    stardec::treebuilder::build_tree(t, g, filters, 2, true, true);


    /*std::cout << g.goal()->get<stardec::belief>().value() << std::endl;
    stardec::belief_update<ARG_TYPE> bel(stardec::fast_ambivalent<ARG_TYPE>);
    bel.update(g.goal());
    std::cout << g.goal()->get<stardec::belief>().value() << std::endl;*/

    /*stardec::affective_norm_update<ARG_TYPE> aff(values[category]);
    auto norm = g.goal()->get<stardec::affective_norm>().value();
    for( auto n : norm )
        std::cout << n << " ";
    std::cout << std::endl;
    aff.update(g.goal());
    norm = g.goal()->get<stardec::affective_norm>().value();
    for( auto n : norm )
        std::cout << n << " ";
    std::cout << std::endl;*/

    /*if(verbose)
        std::cout << "Building tree" << std::endl;*/



    /*if(!options.count("allowduplicate"))
        f_struct->filters.push_back(stardec::remove_duplicate);
    if(!options.count("goalatks"))
        f_struct->filters.push_back(std::bind(stardec::remove_goal_atks, std::cref(g), std::placeholders::_1, std::placeholders::_2));
    if(!options.count("allowirrelevant"))
        f_struct->filters.push_back(std::bind(stardec::relevant, std::cref(g), std::placeholders::_1, std::placeholders::_2));

    if(update == "fast_ambivalent") f_struct->update = stardec::fast_ambivalent;
    f_struct->valuation = std::make_tuple(stardec::present, std::bind(stardec::circumplex, std::placeholders::_1, std::placeholders::_2, std::cref(values.at(category))));*/

    //auto agg = stardec::mc;
    //stardec::tree<std::vector<double>> t(g, f_struct.filters, f_struct.update, valuation, agg, horizon, verbose, mtbuild, true);
    //stardec::tree<stardec::belief, stardec::valuation, stardec::affective_norm> t(std::make_shared<stardec::graph>(&g), f_struct, horizon, mtbuild);

    // std::cout << t.to_dot() << std::endl;
    // return 0;

    // if(verbose)
    //     std::cout << "Computing policy" << std::endl;
    // auto ref = std::bind(stardec::biaised_ideal, std::placeholders::_1, std::vector<double>({1, 0.8, 0.8, 1, 1}));
    // auto dec = std::bind(stardec::rpemomax, std::placeholders::_1, std::cref(ref));
    // t.optimize(dec);
    // stardec::evaluator<std::vector<double>> eval;
    // if(verbose)
    //     std::cout << "Evaluating policy" << std::endl;
    //
    // auto prop = stardec::optimal<std::vector<double>>, opp = stardec::optimal<std::vector<double>>;
    // auto p = eval.evaluate(t.root(), prop, opp);
    // // std::cout << t.to_dot() << std::endl;
    // //for(auto s : p.first)
    //   //std::cout << s << " ";
    // if(verbose) {
    //     std::cout << "Result: ";
    //     for(auto s : p.first)
    //         std::cout << s << " ";
    // }
    return 0;
}
