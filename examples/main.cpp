#include <iostream>
#include "symphony.h"
#include "problems/vacuum.h"
#include "problems/simple_maze.h"
#include "problems/task_scheduler.h"


int main () {
    {
        VacuumCleaner vacuum_cleaner;
        Search *search = create_search(SearchAlgorithmIndex::BREADTH_FIRST_SEARCH, &vacuum_cleaner);
        std::shared_ptr<Node> node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            State *vacuum_state = node->state.get();
            vacuum_state->print();
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    }

    {
        MazeProblem maze_problem;
        Search *search = create_search(SearchAlgorithmIndex::BREADTH_FIRST_SEARCH, &maze_problem);
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

    }

    {
        MazeProblem maze_problem;
        Search *search = create_search(SearchAlgorithmIndex::A_STAR, &maze_problem);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found using A*!" << std::endl;
            State *maze_state = node->state.get();
            maze_state->print();
            Solution solution(node.get());
            solution.print();
        } else {
            std::cout << "Solution not found using A*!" << std::endl;
        }

        delete search;
    }

    {
        TaskScheduler task_scheduler;
        Search *search = create_search(SearchAlgorithmIndex::A_STAR, &task_scheduler);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found using A*!" << std::endl;
            State *task_scheduler_state = node->state.get();
            task_scheduler_state->print();
            Solution solution(node.get());
            solution.print();
        } else {
            std::cout << "Solution not found using A*!" << std::endl;
        }
    }

}
