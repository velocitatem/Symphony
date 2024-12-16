#ifndef SEARCH_H
#define SEARCH_H


#include <map>
#include "definitions.h"
#include <memory>


/* @brief Node class for search algorithms.
 *
 * This class represents a node in the search tree, which contains a state, a parent node, an action, the path cost to reach this node, and a heuristic value.
 * The parent node is a shared pointer to prevent circular references and memory leaks.
 * The state is a smart pointer to manage memory and prevent memory leaks.
 * The action is a pointer to an immutable Action object.
 */
class Node {
public:
    /* @brief Smart pointer to manage parent memory
     */
    std::shared_ptr<Node> parent; // Weak pointer to prevent circular references
    /* @brief Smart pointer to manage state memory.
     */
    std::shared_ptr<State> state; // Smart pointer to manage state memory
    /* @brief Pointer to an immutable Action object.
     */
    const std::shared_ptr<Action> action;        // Pointer to an immutable Action object
    double path_cost;            // Cost to reach this node
    double heuristic;            // Heuristic value (if applicable)

    /* @brief Default constructor for the Node class.
     */
    Node() : action(nullptr), path_cost(0), heuristic(0) {}

    // Parameterized constructor
    Node(std::shared_ptr<Node> parent, std::shared_ptr<State> state, const std::shared_ptr<Action> action, double path_cost, double heuristic)
        : parent(parent), state(state), action(action), path_cost(path_cost), heuristic(heuristic) {}
};


/* @brief Abstract class for search algorithms.
 *
 * This class defines the structure of a search algorithm, which is used to explore a problem space and find a solution. The search algorithm is responsible for traversing the graph of states and actions to find a path from the initial state to a goal state.
 * All Symphony search algorithms should inherit from this class and implement the search method.
 * All methods must use smart pointers to manage memory and prevent memory leaks, they are all responsible for freeing the memory they allocate.
 */
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

/**
 * @brief Breadth-first search algorithm implementation.
 *
 * This class implements the breadth-first search algorithm, which explores a graph by visiting all the neighbor nodes at the present depth prior to moving on to the nodes at the next depth level.
 */
class BreadthFirstSearch : public Search {
public:
    BreadthFirstSearch(Problem *problem) : Search(problem) {}
    /**
    * @brief Breadth-first search algorithm implementation.
    * The breadth-first search algorithm explores a graph by visiting all the neighbor nodes at the present depth prior to moving on to the nodes at the next depth level.
    */
    std::shared_ptr<Node> search() override;
    ~BreadthFirstSearch();
};

class AStarSearch : public Search {
public:
    AStarSearch(Problem *problem) : Search(problem) {}
    std::shared_ptr<Node> search() override;
    ~AStarSearch();
};

/* @brief Beam search algorithm implementation.
 *
 * This class implements the beam search algorithm, which is a heuristic search algorithm that explores a graph by expanding the most promising nodes in a limited set of nodes called the beam width.
 */
class BeamSearch : public Search {
public:
    BeamSearch(Problem *problem, int beam_width) : Search(problem), beam_width(beam_width) {}
    /* @brief Beam search algorithm implementation.
     *
     * This class implements the beam search algorithm, which is a heuristic search algorithm that explores a graph by expanding the most promising nodes in the search tree.
     * Beam search is one approach to limit the memory used by A* search. Beam search limits the size of the frontier, keeping only the k best f(n) nodes.  A good selection of k will help make beam search fast and potentially near-optimal. Beam search is incomplete and sub-optimal. Another option for beam search is to keep all nodes with f(n') > d f(n) where f(n) is the current best.
     */
    std::shared_ptr<Node> search() override;
    ~BeamSearch() override;
    int beam_width;
};

enum SearchAlgorithmIndex {
    BREADTH_FIRST_SEARCH,
    UNIFORM_COST_SEARCH,
    A_STAR,
    BEAM_SEARCH
};

/**
 * @brief Factory method to create a search object based on the specified search algorithm.
 *
 * @param search_algorithm_index The index of the search algorithm to use.
 * @param problem The problem to solve.
 * @return A pointer to the created search object.
 */
Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem); // DEFINED IN search.cpp

#endif //SEARCH_H
