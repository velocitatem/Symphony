#include "definitions.h"
#include <iostream>
#include <unordered_map>

int main() {
    // Create a CSP problem
    CSPProblem csp;

    // Add variables and their domains
    csp.add_variable("X", {1, 2, 3});
    csp.add_variable("Y", {1, 2, 3});
    csp.add_variable("Z", {1, 2, 3});

    // Add constraints
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("X") != assignment.at("Y");
    });
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("Y") != assignment.at("Z");
    });
    csp.add_constraint([](const std::unordered_map<std::string, int> &assignment) {
        return assignment.at("X") != assignment.at("Z");
    });

    // Solve the CSP using backtracking search
    BacktrackingSearch search(&csp);
    std::unordered_map<std::string, int> solution = search.search();

    // Print the solution
    if (!solution.empty()) {
        std::cout << "Solution found:" << std::endl;
        for (const auto &pair : solution) {
            std::cout << pair.first << " = " << pair.second << std::endl;
        }
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}
