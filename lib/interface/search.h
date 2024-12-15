//
// Created by velocitatem on 12/15/24.
//

#ifndef SEARCH_H
#define SEARCH_H



#include "definitions.h"

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
    virtual Node *search() = 0;
    Problem *problem;
};


class BreadthFirstSearch : public Search {
public:
    BreadthFirstSearch(Problem *problem) : Search(problem) {}
    Node *search() override;
    ~BreadthFirstSearch();


};




#endif //SEARCH_H
