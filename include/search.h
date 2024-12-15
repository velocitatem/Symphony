//
// Created by velocitatem on 12/15/24.
//

#ifndef SEARCH_H
#define SEARCH_H


#include <map>
#include "definitions.h"
#include <memory>

class Node {
public:
    Node(Node *parent, State *state, Action *action, double path_cost, double heuristic) : parent(parent), state(state), action(action), path_cost(path_cost), heuristic(heuristic) {}
    Node *parent;
    State *state;
    Action *action;
    double path_cost;
    double heuristic;
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




enum SearchAlgorithmIndex {
    BREADTH_FIRST_SEARCH,
    UNIFORM_COST_SEARCH,
    A_STAR
};

Search *create_search(SearchAlgorithmIndex search_algorithm_index, Problem *problem);




#endif //SEARCH_H
