//
// Created by velocitatem on 12/16/24.
//


#ifndef VACUUM_H
#define VACUUM_H

#include <iostream>
#include "../symphony.h"


class VacuumState : public State {
public:
    VacuumState() : x(0), dirty0(true), dirty1(true) {}
    VacuumState(int x, bool dirty0, bool dirty1) : x(x), dirty0(dirty0), dirty1(dirty1) {}
    int x;
    bool dirty0;
    bool dirty1;
    void print() override {
        std::cout << "VacuumState(" << x << ", " << dirty0 << ", " << dirty1 << ")" << std::endl;
    }
};

class VacuumCleaner : public Problem {
public:
    VacuumCleaner() {
        initial_state_ = new VacuumState();
    }
    ~VacuumCleaner() {
    }
    bool goal_test(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        /// nul.l check
        if (vacuum_state == nullptr) {
            return false;
        }
        return !vacuum_state->dirty0 && !vacuum_state->dirty1;
    }
    std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) override {
        auto vacuum_state = std::dynamic_pointer_cast<VacuumState>(state);
        std::vector<std::shared_ptr<Action>> actions;

        auto create_state = [&](int x, bool dirty0, bool dirty1) {
            return std::make_shared<VacuumState>(x, dirty0, dirty1);
        };

        // Add possible actions with smart pointers
        if (vacuum_state->x == 0) {
            if (vacuum_state->dirty0) {
                actions.push_back(std::make_shared<Action>("Suck", 1, state, create_state(0, false, vacuum_state->dirty1)));
            } else {
                actions.push_back(std::make_shared<Action>("Right", 1, state, create_state(1, vacuum_state->dirty0, vacuum_state->dirty1)));
            }
        } else {
            if (vacuum_state->dirty1) {
                actions.push_back(std::make_shared<Action>("Suck", 1, state, create_state(1, vacuum_state->dirty0, false)));
            } else {
                actions.push_back(std::make_shared<Action>("Left", 1, state, create_state(0, vacuum_state->dirty0, vacuum_state->dirty1)));
            }
        }

        return actions;
    }

    double heuristic(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        return vacuum_state->dirty0 + vacuum_state->dirty1;
    }
};


#endif
