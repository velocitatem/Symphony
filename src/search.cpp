//
// Created by velocitatem on 12/15/24.
//

#include "search.h"
#include "utils.cpp"
#include <queue>


Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem) {
    switch (search_algorithm_index) {
        case BREADTH_FIRST_SEARCH:
            return new BreadthFirstSearch(problem);
        case UNIFORM_COST_SEARCH:
            return new UniformCostSearch(problem);
        case A_STAR:
            return new AStar(problem);
        default:
            return nullptr;
    }
}


Node *BreadthFirstSearch::search() {
    std::queue<Node *> frontier;
    frontier.push(new Node(nullptr, problem->initial_state(), nullptr, 0, 0));
    while (!frontier.empty()) {
        auto *node = frontier.front();
        frontier.pop();
        if (problem->goal_test(node->state)) {
            return node;
        }
        for (auto &action : problem->actions(node->state)) {
            auto *child = new Node(node, action.effect, &action, node->path_cost + action.cost, 0);
            frontier.push(child);
        }
    }
    return nullptr;
}

BreadthFirstSearch::~BreadthFirstSearch() { }


Node *UniformCostSearch::search() {
    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> frontier;
    frontier.push(new Node(nullptr, problem->initial_state(), nullptr, 0, 0));
    while (!frontier.empty()) {
        auto *node = frontier.top();
        frontier.pop();
        if (problem->goal_test(node->state)) {
            return node;
        }
        for (auto &action : problem->actions(node->state)) {
            auto *child = new Node(node, action.effect, &action, node->path_cost + action.cost, 0);
            frontier.push(child);
        }
    }
    return nullptr;
}

Node *AStar::search() {
    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> frontier;
    frontier.push(new Node(nullptr, problem->initial_state(), nullptr, 0, problem->heuristic(problem->initial_state())));
    while (!frontier.empty()) {
        auto *node = frontier.top();
        frontier.pop();
        if (problem->goal_test(node->state)) {
            return node;
        }
        for (auto &action : problem->actions(node->state)) {
            auto *child = new Node(node, action.effect, &action, node->path_cost + action.cost, problem->heuristic(action.effect));
            frontier.push(child);
        }
    }
    return nullptr;
}


