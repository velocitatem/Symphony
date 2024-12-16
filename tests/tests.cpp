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

// Test case for CSPProblem class
TEST(CSPProblem, AddVariable) {
    CSPProblem csp;
    csp.add_variable("X", {1, 2, 3});
    EXPECT_EQ(csp.variables().size(), 1);
    EXPECT_EQ(csp.variables().at("X").size(), 3);
}

TEST(CSPProblem, AddConstraint) {
    CSPProblem csp;
    csp.add_variable("X", {1, 2, 3});
    csp.add_variable("Y", {1, 2, 3});
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("X") != assignment.at("Y");
    });
    std::unordered_map<std::string, int> assignment = {{"X", 1}, {"Y", 2}};
    EXPECT_TRUE(csp.is_consistent(assignment));
    assignment = {{"X", 1}, {"Y", 1}};
    EXPECT_FALSE(csp.is_consistent(assignment));
}

// Test case for BacktrackingSearch class
TEST(BacktrackingSearch, Search) {
    CSPProblem csp;
    csp.add_variable("X", {1, 2, 3});
    csp.add_variable("Y", {1, 2, 3});
    csp.add_variable("Z", {1, 2, 3});
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("X") != assignment.at("Y");
    });
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("Y") != assignment.at("Z");
    });
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("X") != assignment.at("Z");
    });

    BacktrackingSearch search(&csp);
    std::unordered_map<std::string, int> solution = search.search();

    EXPECT_FALSE(solution.empty());
    EXPECT_NE(solution["X"], solution["Y"]);
    EXPECT_NE(solution["Y"], solution["Z"]);
    EXPECT_NE(solution["X"], solution["Z"]);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
