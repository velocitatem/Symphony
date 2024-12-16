//
// Created by velocitatem on 12/16/24.
//


#ifndef VACUUM_H
#define VACUUM_H

#include <iostream>
#include "../symphony.h"




/**
 * @brief Represents the state of the vacuum cleaner problem.
 */
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


/**
 * @brief Represents the vacuum cleaner problem.
 * This class defines the initial state, goal test, actions, and heuristics for the vacuum cleaner problem.
 * The problem is to clean two dirty rooms with a vacuum cleaner. The goal is to clean both rooms.
 */
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
    /**
     * @brief Returns the possible actions for the given state.
     * @param state The current state.
     * @return A vector of shared pointers to Action objects.
     */
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

    /**
     * @brief Returns the heuristic value for the given state.
     * @param state The current state.
     * @return The heuristic value.
     */
    double heuristic(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        return vacuum_state->dirty0 + vacuum_state->dirty1;
    }
};


#endif
