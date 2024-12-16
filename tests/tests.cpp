#include <gtest/gtest.h>
#include "definitions.h"
#include "search.h"

class TestState : public State {
public:
    int value;
    TestState(int value) : value(value) {}
    void print() override {}
};

class TestProblem : public Problem {
public:
    TestProblem() {
        initial_state_ = new TestState(0);
    }
    ~TestProblem() {
    }
    bool goal_test(State *state) override {
        auto *test_state = dynamic_cast<TestState *>(state);
        return test_state->value == 10;
    }
    std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) override {
        auto test_state = std::dynamic_pointer_cast<TestState>(state);
        std::vector<std::shared_ptr<Action>> actions;
        actions.push_back(std::make_shared<Action>("Increment", 1, state, std::make_shared<TestState>(test_state->value + 1)));
        return actions;
    }
    double heuristic(State *state) override {
        auto *test_state = dynamic_cast<TestState *>(state);
        return 10 - test_state->value;
    }
};

TEST(Definitions, State) {
    State state;
    EXPECT_NO_THROW(state.print());
}

TEST(Search, AStarSearch) {
    TestProblem problem;
    Search *search = create_search(SearchAlgorithmIndex::A_STAR, &problem);
    std::shared_ptr<Node> node = search->search();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(dynamic_cast<TestState *>(node->state.get())->value, 10);
    delete search;
}

TEST(Search, BreadthFirstSearch) {
    TestProblem problem;
    Search *search = create_search(SearchAlgorithmIndex::BREADTH_FIRST_SEARCH, &problem);
    std::shared_ptr<Node> node = search->search();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(dynamic_cast<TestState *>(node->state.get())->value, 10);
    delete search;
}

TEST(Search, BeamSearch) {
    TestProblem problem;
    Search *search = create_search(SearchAlgorithmIndex::BEAM_SEARCH, &problem);
    std::shared_ptr<Node> node = search->search();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(dynamic_cast<TestState *>(node->state.get())->value, 10);
    delete search;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
