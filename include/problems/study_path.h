//
// Created by velocitatem on 12/16/24.
//

#ifndef STUDY_PATH_H
#define STUDY_PATH_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "symphony.h"



// Study-specific classes
class StudyState : public State {
public:
    std::map<std::string, double> mastery_levels; // Topic -> Mastery %
    double remaining_time;

    StudyState(std::map<std::string, double> mastery_levels, double remaining_time)
        : mastery_levels(std::move(mastery_levels)), remaining_time(remaining_time) {}

    void print() override {
        for (const auto& [topic, mastery] : mastery_levels) {
            std::cout << topic << ": " << mastery << "%\n";
        }
        std::cout << "Time left: " << remaining_time << " hours\n";
    }
};

class StudyProblem : public Problem {
    StudyState* initial_state_;
    std::map<std::string, std::vector<std::string>> dependencies; // Topic -> Prerequisites
    std::map<std::string, double> synergies; // Topic synergy weights

public:
    StudyProblem(StudyState* initial_state,
                 std::map<std::string, std::vector<std::string>> dependencies,
                 std::map<std::string, double> synergies)
        : initial_state_(initial_state), dependencies(std::move(dependencies)), synergies(std::move(synergies)) {}

    State* initial_state() override {
        return initial_state_;
    }

    bool goal_test(State* state) override {
        auto* study_state = dynamic_cast<StudyState*>(state);
        for (const auto& [_, mastery] : study_state->mastery_levels) {
            if (mastery < 100.0) return false;
        }
        return true;
    }

    std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) override {
        auto* study_state = std::dynamic_pointer_cast<StudyState>(state).get();
        std::vector<std::shared_ptr<Action>> available_actions;

        for (const auto& [topic, mastery] : study_state->mastery_levels) {
            if (mastery < 100.0 && study_state->remaining_time > 0) {
                double cost = 1.0; // 1 hour per study session
                auto new_mastery = study_state->mastery_levels;
                new_mastery[topic] += std::min(10.0, 100.0 - mastery); // Increment by 10%, cap at 100%
                double synergy_bonus = synergies.count(topic) ? synergies.at(topic) : 0.0;
                new_mastery[topic] += synergy_bonus;

                double time_left = study_state->remaining_time - cost;
                auto new_state = new StudyState(new_mastery, time_left);
                available_actions.emplace_back( std::make_shared<Action>(topic, cost, state, std::shared_ptr<State>(new_state)) );
            }
        }

        return available_actions;
    }

    double heuristic(State* state) override {
        auto* study_state = dynamic_cast<StudyState*>(state);
        double total_gap = 0;
        for (const auto& [_, mastery] : study_state->mastery_levels) {
            total_gap += (100.0 - mastery);
        }
        return total_gap / study_state->remaining_time;
    }
};

// Main Function
void run(std::string &input) {
    std::ifstream file(input);
    std::string json_string;
    std::string line;
    while (std::getline(file, line)) {
        json_string += line;
    }

    nlohmann::json json = nlohmann::json::parse(json_string);
    std::map<std::string, double> mastery_levels = json["mastery_levels"];
    std::map<std::string, std::vector<std::string>> dependencies = json["dependencies"];
    std::map<std::string, double> synergies = json["synergies"];
    double time = json["time"];


    auto initial_state = new StudyState(mastery_levels, time); // 7 days, 3 hours/day
    auto problem = new StudyProblem(initial_state, dependencies, synergies);
    auto solution = create_search(BEAM_SEARCH, problem)->search();

    if (solution) {
        std::cout << "Optimal study plan:\n";
        auto current = solution;
        while (current) {
            if (current->action) {
                std::cout << "Study " << current->action->name << " for " << current->action->cost << " hours\n";
            }
            current = current->parent;
        }
    } else {
        std::cout << "No solution found within the given time.\n";
    }
    delete problem;

}


#endif //STUDY_PATH_H
