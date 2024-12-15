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
    auto current = node;
    std::vector<Action> actions;
    while (current) {
        if (current->action) {
            actions.push_back(*current->action);
        }
        current = current->parent;
    }

    for (auto it = actions.rbegin(); it != actions.rend(); ++it) {
        std::cout << it->name << std::endl;
    }

}


std::shared_ptr<Node> BreadthFirstSearch::search() {
    std::queue<std::shared_ptr<Node>> frontier;
    frontier.push(std::make_shared<Node>(nullptr, problem->initial_state(), nullptr, 0, 0));
    while (!frontier.empty()) {
        auto node = std::move(frontier.front());
        frontier.pop();
        if (problem->goal_test(node->state)) {
            return node;
        }
        for (auto &action : problem->actions(node->state)) {
            auto child = std::make_shared<Node>(node.get(), action.effect, &action, node->path_cost + action.cost, problem->heuristic(action.effect));
            frontier.push(std::move(child));
        }
    }
    return nullptr;
}