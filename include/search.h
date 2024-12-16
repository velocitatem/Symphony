#include "search.h"
#include <map>
#include "definitions.h"
#include <memory>

class Node {
public:
    std::shared_ptr<Node> parent; // Weak pointer to prevent circular references
    std::shared_ptr<State> state; // Smart pointer to manage state memory
    const std::shared_ptr<Action> action;        // Pointer to an immutable Action object
    double path_cost;            // Cost to reach this node
    double heuristic;            // Heuristic value (if applicable)

    // Default constructor
    Node() : action(nullptr), path_cost(0), heuristic(0) {}

    // Parameterized constructor
    Node(std::shared_ptr<Node> parent, std::shared_ptr<State> state, const std::shared_ptr<Action> action, double path_cost, double heuristic)
        : parent(parent), state(state), action(action), path_cost(path_cost), heuristic(heuristic) {}
};


class Search {
public:
    Search(Problem *problem) : problem(problem) {}
    virtual ~Search() {}
    virtual std::shared_ptr<Node> search() = 0;
    Problem *problem;
};

class Solution {
public:
    Solution(Node *node) : node(node) {}
    void print();
    Node *node;
};

class BreadthFirstSearch : public Search {
public:
    BreadthFirstSearch(Problem *problem) : Search(problem) {}
    std::shared_ptr<Node> search() override;
    ~BreadthFirstSearch();
};

class AStarSearch : public Search {
public:
    AStarSearch(Problem *problem) : Search(problem) {}
    std::shared_ptr<Node> search() override;
    ~AStarSearch();
};

enum SearchAlgorithmIndex {
    BREADTH_FIRST_SEARCH,
    UNIFORM_COST_SEARCH,
    A_STAR
};

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

#endif //SEARCH_H
