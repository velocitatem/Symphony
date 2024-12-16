#include "search.h"
#include "utils.cpp"
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_set>

Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem) {
    switch (search_algorithm_index) {
        case BREADTH_FIRST_SEARCH:
            return new BreadthFirstSearch(problem);
        case A_STAR:
            return new AStarSearch(problem);
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

// Implement CSPProblem class methods for handling variables, domains, and constraints
void CSPProblem::add_variable(const std::string &variable, const std::vector<int> &domain) {
    variables_[variable] = domain;
}

void CSPProblem::add_constraint(const std::function<bool(const std::unordered_map<std::string, int> &)> &constraint) {
    constraints_.push_back(constraint);
}

bool CSPProblem::is_consistent(const std::unordered_map<std::string, int> &assignment) const {
    for (const auto &constraint : constraints_) {
        if (!constraint(assignment)) {
            return false;
        }
    }
    return true;
}

const std::unordered_map<std::string, std::vector<int>> &CSPProblem::variables() const {
    return variables_;
}

// Implement BacktrackingSearch class methods for solving CSPs using backtracking search algorithm
BacktrackingSearch::BacktrackingSearch(CSPProblem *problem) : problem_(problem) {}

std::unordered_map<std::string, int> BacktrackingSearch::search() {
    std::unordered_map<std::string, int> assignment;
    if (backtrack(assignment)) {
        return assignment;
    } else {
        return {};
    }
}

bool BacktrackingSearch::backtrack(std::unordered_map<std::string, int> &assignment) {
    if (assignment.size() == problem_->variables().size()) {
        return true;
    }

    std::string variable = select_unassigned_variable(assignment);
    for (int value : problem_->variables().at(variable)) {
        assignment[variable] = value;
        if (problem_->is_consistent(assignment)) {
            if (backtrack(assignment)) {
                return true;
            }
        }
        assignment.erase(variable);
    }

    return false;
}

std::string BacktrackingSearch::select_unassigned_variable(const std::unordered_map<std::string, int> &assignment) const {
    for (const auto &variable : problem_->variables()) {
        if (assignment.find(variable.first) == assignment.end()) {
            return variable.first;
        }
    }
    return "";
}
