#include <iostream>
#include <vector>
#include <algorithm>
#include <argparse/argparse.hpp>
#include "symphony.h"
#include "problems/vacuum.h"
#include "problems/simple_maze.h"
#include "problems/task_scheduler.h"
#include "problems/study_path.h"



int main(int argc, char **argv) {
    argparse::ArgumentParser program("Symphony Examples");

    program.add_argument("problem")
        .help("The problem to solve")
        .default_value(std::string("vacuum"))
        .action([](const std::string &value) {
            static const std::vector<std::string> choices = {"vacuum", "maze", "task_scheduler", "study_path"};
            if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                return value;
            }
            return std::string("vacuum");
        });

    program.add_argument("algorithm")
        .help("The search algorithm to use")
        .default_value(std::string("breadth_first_search"))
        .action([](const std::string &value) {
            static const std::vector<std::string> choices = {"breadth_first_search", "a_star", "beam_search"};
            if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                return value;
            }
            return std::string("breadth_first_search");
        });

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::string problem = program.get<std::string>("problem");
    std::string algorithm = program.get<std::string>("algorithm");

    SearchAlgorithmIndex algorithm_index;
    if (algorithm == "breadth_first_search") {
        algorithm_index = SearchAlgorithmIndex::BREADTH_FIRST_SEARCH;
    } else if (algorithm == "a_star") {
        algorithm_index = SearchAlgorithmIndex::A_STAR;
    } else if (algorithm == "beam_search") {
        algorithm_index = SearchAlgorithmIndex::BEAM_SEARCH;
    } else {
        std::cerr << "Unknown algorithm: " << algorithm << std::endl;
        return 1;
    }

    if (problem == "vacuum") {
        VacuumCleaner vacuum_cleaner;
        Search *search = create_search(algorithm_index, &vacuum_cleaner);
        std::shared_ptr<Node> node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            State *vacuum_state = node->state.get();
            vacuum_state->print();
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    } else if (problem == "maze") {
        MazeProblem maze_problem;
        Search *search = create_search(algorithm_index, &maze_problem);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            State *maze_state = node->state.get();
            maze_state->print();
            Solution solution(node.get());
            solution.print();
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    } else if (problem == "task_scheduler") {
        TaskScheduler task_scheduler;
        Search *search = create_search(algorithm_index, &task_scheduler);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            State *task_scheduler_state = node->state.get();
            task_scheduler_state->print();
            Solution solution(node.get());
            solution.print();
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    } else if (problem == "study_path") {
        std::string path = "/home/velocitatem/CLionProjects/ai_rational_agents/examples/study_plan_++.json"; //TODO: Change this to the path of the json file
        run(path);
    } else {
        std::cerr << "Unknown problem: " << problem << std::endl;
        return 1;
    }

    return 0;
}

