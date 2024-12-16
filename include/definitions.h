/**
 * @file definitions.h
 * @brief Abstract classes for states, actions, and problems that need to be implemented by the user.
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

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
    virtual std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) = 0;

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

/**
 * @brief Represents a constraint satisfaction problem (CSP).
 *
 * This class defines the structure of a CSP, including variables, domains, and constraints.
 * It should be extended by the user to implement specific CSP logic.
 */
class CSPProblem : public Problem {
public:
    /**
     * @brief Constructor for the CSPProblem class.
     */
    CSPProblem() {}

    /**
     * @brief Virtual destructor for the CSPProblem class.
     */
    virtual ~CSPProblem() {}

    /**
     * @brief Adds a variable to the CSP.
     *
     * @param variable The name of the variable.
     * @param domain The domain of the variable.
     */
    void add_variable(const std::string &variable, const std::vector<int> &domain) {
        variables_[variable] = domain;
    }

    /**
     * @brief Adds a constraint to the CSP.
     *
     * @param constraint The constraint function.
     */
    void add_constraint(const std::function<bool(const std::unordered_map<std::string, int> &)> &constraint) {
        constraints_.push_back(constraint);
    }

    /**
     * @brief Tests if a given assignment satisfies all constraints.
     *
     * @param assignment The assignment to test.
     * @return True if the assignment satisfies all constraints, false otherwise.
     */
    bool is_consistent(const std::unordered_map<std::string, int> &assignment) const {
        for (const auto &constraint : constraints_) {
            if (!constraint(assignment)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Retrieves the variables of the CSP.
     *
     * @return A reference to the variables of the CSP.
     */
    const std::unordered_map<std::string, std::vector<int>> &variables() const {
        return variables_;
    }

private:
    std::unordered_map<std::string, std::vector<int>> variables_; ///< Variables of the CSP.
    std::vector<std::function<bool(const std::unordered_map<std::string, int> &)>> constraints_; ///< Constraints of the CSP.
};

/**
 * @brief Represents a backtracking search algorithm for solving CSPs.
 */
class BacktrackingSearch {
public:
    /**
     * @brief Constructor for the BacktrackingSearch class.
     *
     * @param problem The CSP problem to solve.
     */
    BacktrackingSearch(CSPProblem *problem) : problem_(problem) {}

    /**
     * @brief Solves the CSP using backtracking search.
     *
     * @return A solution to the CSP, or an empty assignment if no solution is found.
     */
    std::unordered_map<std::string, int> search() {
        std::unordered_map<std::string, int> assignment;
        if (backtrack(assignment)) {
            return assignment;
        } else {
            return {};
        }
    }

private:
    /**
     * @brief Performs backtracking search to find a solution to the CSP.
     *
     * @param assignment The current assignment.
     * @return True if a solution is found, false otherwise.
     */
    bool backtrack(std::unordered_map<std::string, int> &assignment) {
        if (assignment.size() == problem_->variables().size()) {
            return true;
        }

        std::string variable = select_unassigned_variable(assignment);
        for (int value : problem_->variables().at(variable)) {
            assignment[variable] = value;
            if (problem_->is_consistent(assignment)) {
                if (backtrack(assignment)) {
                    return true;
                }
            }
            assignment.erase(variable);
        }

        return false;
    }

    /**
     * @brief Selects an unassigned variable from the CSP.
     *
     * @param assignment The current assignment.
     * @return The name of an unassigned variable.
     */
    std::string select_unassigned_variable(const std::unordered_map<std::string, int> &assignment) const {
        for (const auto &variable : problem_->variables()) {
            if (assignment.find(variable.first) == assignment.end()) {
                return variable.first;
            }
        }
        return "";
    }

    CSPProblem *problem_; ///< The CSP problem to solve.
};

#endif // DEFINITIONS_H
