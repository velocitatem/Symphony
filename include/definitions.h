/**
 * @file definitions.h
 * @brief Abstract classes for states, actions, and problems that need to be implemented by the user.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <vector>
#include <memory>

/**
 * @brief Represents an abstract state in a problem.
 *
 * This class should be extended by the user to define the specific attributes and behaviors of a state in the problem domain.
 */
class State {
public:
    /**
     * @brief Constructor for the State class.
     */
    State() {}

    /**
     * @brief Virtual destructor for the State class.
     */
    virtual ~State() {}

    /**
     * @brief Prints the state details.
     *
     * This method should be overridden by the user to display meaningful information about the state.
     */
    virtual void print() {}
};

/**
 * @brief Represents an abstract action that can be performed on a state.
 *
 * This class should be extended by the user to define specific actions in the problem domain.
 */
class Action {
public:
 std::string name;                          // Action name (e.g., "Up", "Down")
 double cost;                               // Cost of the action
 std::shared_ptr<State> source_state;       // Source state
 std::shared_ptr<State> effect;            // Resulting state after applying the action

 // Constructor
 Action(const std::string &name, double cost, std::shared_ptr<State> source_state, std::shared_ptr<State> effect)
     : name(name), cost(cost), source_state(source_state), effect(effect) {}
};



/**
 * @brief Represents an abstract problem that needs to be solved.
 *
 * This class defines the structure of a problem, including its initial state, goal test, actions, and heuristics.
 * It should be extended by the user to implement specific problem-solving logic.
 */
class Problem {
public:
    /**
     * @brief Constructor for the Problem class.
     */
    Problem() {}

    /**
     * @brief Virtual destructor for the Problem class.
     */
    virtual ~Problem() {}

    /**
     * @brief Retrieves the initial state of the problem.
     *
     * @return A pointer to the initial state.
     */
    virtual State *initial_state() { return initial_state_; }

    /**
     * @brief Tests if a given state satisfies the goal condition.
     *
     * This method must be implemented by the user to define the goal state criteria.
     *
     * @param state The state to test.
     * @return True if the state satisfies the goal condition, false otherwise.
     */
    virtual bool goal_test(State *state) = 0;

    /**
     * @brief Retrieves the set of actions applicable to a given state.
     *
     * This method must be implemented by the user to define the available actions for a specific state.
     *
     * @param state The current state.
     * @return A vector of actions applicable to the given state.
     */
    virtual std::vector<Action> actions(std::shared_ptr<State> state) = 0;

    /**
     * @brief Computes a heuristic estimate for a given state.
     *
     * This method must be implemented by the user to define a heuristic function for the problem.
     *
     * @param state The state for which to compute the heuristic.
     * @return A heuristic estimate of the cost to reach the goal from the given state.
     */
    virtual double heuristic(State *state) = 0;

    /// Pointer to the initial state of the problem.
    State *initial_state_;
};

#endif // DEFINITIONS_H
