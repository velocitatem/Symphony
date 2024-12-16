//
// Created by velocitatem on 12/15/24.
//

#include "search.h"
#include "utils.cpp"
#include <queue>
#include <memory>
#include <iostream>

Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem) {
    switch (search_algorithm_index) {
        case BREADTH_FIRST_SEARCH:
            return new BreadthFirstSearch(problem);
        default:
            return nullptr;
    }
}

BreadthFirstSearch::~BreadthFirstSearch() { }

void Solution::print() {
    Node * current = this->node;
    std::vector<Action> actions;
    auto parent = current->parent;
    // Traverse the solution path by following the parent pointers
    while (parent != nullptr) {
        actions.push_back(*current->action);
        current = parent.get();
        parent = current->parent;
    }

    for (auto it = actions.rbegin(); it != actions.rend(); ++it) {
        std::cout << it->name << std::endl;
    }

}

std::shared_ptr<Node> BreadthFirstSearch::search() {
    std::queue<std::shared_ptr<Node>> frontier;
    // Initialize the root node with the problem's initial state
    auto initial_state = problem->initial_state();
    auto root = std::make_shared<Node>(
        nullptr,                    // Parent node
        std::shared_ptr<State>(initial_state), // Initial state (shared_ptr)
        nullptr,                    // No action
        0,                          // Path cost
        problem->heuristic(initial_state) // Heuristic value
    );
    frontier.push(root);


    while (!frontier.empty()) {
        auto node = frontier.front();
        frontier.pop();

        // Check if the goal state is reached
        State *state = node->state.get();
        if (problem->goal_test(state)) {
            return node;
        }

        // Expand the node by generating its child nodes
        for (const auto &action : problem->actions(node->state)) {
            auto child = std::make_shared<Node>(
                std::shared_ptr<Node>(node), // Parent node
                action->effect,
                action,                    // Pointer to the action
                node->path_cost + action->cost, // Path cost
                problem->heuristic(action->effect.get()) // Heuristic value
            );
            frontier.push(child);
        }
    }

    // Return nullptr if no solution is found
    return nullptr;
}
