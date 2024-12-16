#include "search.h"
#include "utils.cpp"
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <functional>


Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem) {
    switch (search_algorithm_index) {
        case BREADTH_FIRST_SEARCH:
            return new BreadthFirstSearch(problem);
        case A_STAR:
            return new AStarSearch(problem);
        case BEAM_SEARCH:
            return new BeamSearch(problem, 2);
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
    auto initial_state = problem->initial_state();
    auto root = std::make_shared<Node>(
        nullptr,
        std::shared_ptr<State>(initial_state),
        nullptr,
        0,
        problem->heuristic(initial_state)
    );
    frontier.push(root);
    while (!frontier.empty()) {
        auto node = frontier.front();
        frontier.pop();

        State *state = node->state.get();
        if (problem->goal_test(state)) {
            return node;
        }
        for (const auto &action : problem->actions(node->state)) {
            auto child = std::make_shared<Node>(
                std::shared_ptr<Node>(node),
                action->effect,
                action,
                node->path_cost + action->cost,
                problem->heuristic(action->effect.get())
            );
            frontier.push(child);
        }
    }
    // Return nullptr if no solution is found
    return nullptr;
}

AStarSearch::~AStarSearch() { }

std::shared_ptr<Node> AStarSearch::search() {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> frontier;
    // Initialize the root node with the problem's initial state
    std::unordered_set<std::shared_ptr<State>> explored; // Set of explored states
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
        auto node = frontier.top();
        frontier.pop();

        // Check if the goal state is reached
        State *state = node->state.get();
        if (problem->goal_test(state)) {
            return node;
        }

        // Skip the node if it has already been explored
        if (explored.find(node->state) != explored.end()) {
            continue;
        }
        explored.insert(node->state);

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




BeamSearch::~BeamSearch() { }

std::shared_ptr<Node> BeamSearch::search() {
    // Priority queue to maintain the beam
    using Beam = std::vector<std::shared_ptr<Node>>;
    Beam beam;

    // Initialize the root node
    auto initial_state = problem->initial_state();
    auto root = std::make_shared<Node>(
        nullptr,
        std::shared_ptr<State>(initial_state),
        nullptr,
        0,
        problem->heuristic(initial_state)
    );

    beam.push_back(root);

    while (!beam.empty()) {
        // Sort beam nodes based on their f(n) = path_cost + heuristic
        std::sort(beam.begin(), beam.end(), [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return (a->path_cost + a->heuristic) < (b->path_cost + b->heuristic);
        });

        // Keep only the top k nodes (beam width)
        if (beam.size() > beam_width) {
            beam.resize(beam_width);
        }

        Beam next_beam;

        // Expand each node in the current beam
        for (const auto& node : beam) {
            if (problem->goal_test(node->state.get())) {
                return node; // Goal found
            }

            // Generate successors
            for (const auto& action : problem->actions(node->state)) {
                auto child = std::make_shared<Node>(
                    node,
                    action->effect,
                    action,
                    node->path_cost + action->cost,
                    problem->heuristic(action->effect.get())
                );
                next_beam.push_back(child);
            }
        }

        beam = std::move(next_beam);
    }

    return nullptr;
}
