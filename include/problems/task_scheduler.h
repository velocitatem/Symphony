#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <iostream>
#include <vector>
#include <queue>
#include "../symphony.h"



/**
 * @brief Represents a task with a name, priority, and deadline.
 */
class Task {
public:
    Task(std::string name, int priority, int deadline)
        : name(name), priority(priority), deadline(deadline) {}

    bool operator==(const Task &other) const {
        return name == other.name && priority == other.priority && deadline == other.deadline;
    }

    std::string name;
    int priority;
    int deadline;
};

/**
 * @brief Represents the state of the task scheduler problem.
 */
class TaskSchedulerState : public State {
public:
    TaskSchedulerState() {
        tasks = {
            Task("Task 1", 3, 5),
            Task("Task 2", 2, 3),
            Task("Task 3", 5, 10)
        };
    }
    TaskSchedulerState(const TaskSchedulerState &other) {
        tasks = other.tasks;
    }
    TaskSchedulerState(std::vector<Task> tasks) : tasks(tasks) {}
    std::vector<Task> tasks;
    void print() override {
        for (const auto &task : tasks) {
            std::cout << "Task: " << task.name << ", Priority: " << task.priority << ", Deadline: " << task.deadline << std::endl;
        }
    }
};


class TaskScheduler : public Problem {
/**
 * @brief Represents the task scheduler problem.
 * This class defines the initial state, goal test, actions, and heuristics for the task scheduler problem.
 * The problem is to complete a set of tasks with different priorities and deadlines. The goal is to complete all tasks.
 */
public:
    TaskScheduler() {
        initial_state_ = new TaskSchedulerState();
    }
    ~TaskScheduler() {
    }
    bool goal_test(State *state) override {
        auto *scheduler_state = dynamic_cast<TaskSchedulerState *>(state);
        return scheduler_state && scheduler_state->tasks.empty();
    }
    std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) override {
        auto scheduler_state = std::dynamic_pointer_cast<TaskSchedulerState>(state);
        std::vector<std::shared_ptr<Action>> actions;

        for (const auto &task : scheduler_state->tasks) {
            auto new_state = std::make_shared<TaskSchedulerState>(*scheduler_state);
            // Remove the task from the new state
            int index = 0;
            for (const auto &t : new_state->tasks) {
                if (t == task) {
                    new_state->tasks.erase(new_state->tasks.begin() + index);
                    break;
                }
                index++;
            }
            actions.push_back(std::make_shared<Action>("Complete " + task.name, 1, state, new_state));
        }

        return actions;
    }
    double heuristic(State *state) override {
        auto *scheduler_state = dynamic_cast<TaskSchedulerState *>(state);
        int total_priority = 0;
        for (const auto &task : scheduler_state->tasks) {
            total_priority += task.priority;
        }
        return total_priority;
    }
};

#endif