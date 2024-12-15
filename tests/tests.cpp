//
// Created by velocitatem on 12/8/24.
//

#include <gtest/gtest.h>
#include "definitions.h"
#include "search.h"



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
        delete initial_state_;
    }
    bool goal_test(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        return !vacuum_state->dirty0 && !vacuum_state->dirty1;
    }
    std::vector<Action> actions(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        std::vector<Action> actions;
        if (vacuum_state->dirty0 && vacuum_state->x == 0) {
            actions.emplace_back("Suck", 1, vacuum_state, new VacuumState(vacuum_state->x, false, vacuum_state->dirty1));
        }
        if (vacuum_state->dirty1 && vacuum_state->x == 1) {
            actions.emplace_back("Suck", 1, vacuum_state, new VacuumState(vacuum_state->x, vacuum_state->dirty0, false));
        }
        if (vacuum_state->x == 0) {
            actions.emplace_back("Right", 1, vacuum_state, new VacuumState(1, vacuum_state->dirty0, vacuum_state->dirty1));
        } else {
            actions.emplace_back("Left", 1, vacuum_state, new VacuumState(0, vacuum_state->dirty0, vacuum_state->dirty1));
        }
        return actions;
    }
    double heuristic(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        return vacuum_state->dirty0 + vacuum_state->dirty1;
    }
};

TEST(Problems, VacuumCleaner) {

    VacuumCleaner vacuum_cleaner;
    auto *state = vacuum_cleaner.initial_state();
    EXPECT_FALSE(vacuum_cleaner.goal_test(state));
    auto actions = vacuum_cleaner.actions(state);
    for (auto &action : actions) {
        action.effect->print();
    }

}

TEST(Search, BreadthFirstSearch) {
    VacuumCleaner vacuum_cleaner;

    auto *search = create_search(BREADTH_FIRST_SEARCH, &vacuum_cleaner);
    auto node = search->search();

    bool dirty0 = dynamic_cast<VacuumState *>(node->state)->dirty0;
    bool dirty1 = dynamic_cast<VacuumState *>(node->state)->dirty1;
    EXPECT_FALSE(dirty0);
    EXPECT_FALSE(dirty1);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
