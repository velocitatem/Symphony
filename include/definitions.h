//
// Created by velocitatem on 12/15/24.
//

#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>


class State { // abstract class for states
public:
    State() {}
    virtual ~State() {}
    virtual void print() {}
};

class Action { // abstract class for actions
public:
    Action() {}
    Action(std::string name, double cost, State *precondition, State *effect) : name(name), cost(cost), precondition(precondition), effect(effect) {}
    virtual ~Action() {}
    std::pmr::string name;
    double cost;
    State *precondition;
    State *effect;
};

class Problem {
public:
    Problem() {}
    virtual ~Problem() {}
    virtual State *initial_state() { return initial_state_; }
    virtual bool goal_test(State *state) = 0;
    virtual std::vector<Action> actions(State *state) = 0;
    virtual double heuristic (State *state) = 0;
    // VARIABLES
    State *initial_state_;
};


#endif //DEFINITIONS_H
