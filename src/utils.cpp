//
// Created by velocitatem on 12/15/24.
//


#include "search.h"

// node copmarator for priority queue
class NodeComparator {
public:
    bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b) {
        return a->path_cost + a->heuristic > b->path_cost + b->heuristic;
    }
};

