//
// Created by velocitatem on 12/15/24.
//

#include "interface/search.h"
#include "interface/definitions.h"
#include <queue>



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

BreadthFirstSearch::~BreadthFirstSearch() {
}