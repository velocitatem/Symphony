/**
 * @file definitions.h
 * @brief Abstract classes for states, actions, and problems that need to be implemented by the user.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <vector>

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
    /**
     * @brief Default constructor for the Action class.
     */
    Action() {}

    /**
     * @brief Parameterized constructor for the Action class.
     *
     * @param name Name of the action.
     * @param cost Cost of performing the action.
     * @param precondition The precondition state required for this action.
     * @param effect The resulting state after performing this action.
     */
    Action(std::string name, double cost, State *precondition, State *effect)
        : name(name), cost(cost), precondition(precondition), effect(effect) {}

    /**
     * @brief Virtual destructor for the Action class.
     */
    virtual ~Action() {}

    /// Name of the action.
    std::pmr::string name;

    /// Cost of performing the action.
    double cost;

    /// Precondition state required for this action.
    State *precondition;

    /// The resulting state after performing this action.
    State *effect;
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
    virtual std::vector<Action> actions(State *state) = 0;

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
