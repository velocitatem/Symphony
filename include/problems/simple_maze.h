//
// Created by velocitatem on 12/16/24.
//

#ifndef SIMPLE_MAZE_H
#define SIMPLE_MAZE_H

#include <iostream>
#include <vector>
#include "symphony.h"


class MazeState : public State {
// use a 2D array to represent the maze
public:
    MazeState() {
        maze = {
            {0, 0, 0, 0, 0},
            {0, 1, 1, 1, 0},
            {0, 1, -1, 0, 0},
            {0, 1, 1, 1, 0},
            {0, 0, 0, 0, 0}
        };
        x = 1;
        y = 1;
    }
    MazeState(std::vector<std::vector<int>> maze, int x, int y) : maze(maze), x(x), y(y) {}
    std::vector<std::vector<int>> maze;
    int x;
    int y;
    void print() override {
        int row_index = 0;
        for (auto &row : maze) {
            int col_index = 0;
            for (auto &cell : row) {
                if (x == row_index && y == col_index) {
                    std::cout << "V";
                    col_index++;
                    continue;
                }
                if (cell == 0) {
                    std::cout << " ";
                } else if (cell == 1) {
                    std::cout << "#";
                } else if (cell == -1) {
                    std::cout << "X";
                }
                col_index++;
            }
            row_index++;
            std::cout << std::endl;
        }
    }
};


class MazeProblem : public Problem {
public:
    MazeProblem() {
        initial_state_ = new MazeState();
    }
    ~MazeProblem() {
    }
    bool goal_test(State *state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state);
        return maze_state->maze[maze_state->x][maze_state->y] == -1;
    }

    std::vector<std::shared_ptr<Action>> actions(std::shared_ptr<State> state) override {
        auto maze_state = std::dynamic_pointer_cast<MazeState>(state);
        std::vector<std::shared_ptr<Action>> actions;

        auto create_state = [&](int x, int y) {
            return std::make_shared<MazeState>(maze_state->maze, x, y);
        };

        // Add possible actions with smart pointers
        if (maze_state->x > 0 && maze_state->maze[maze_state->x - 1][maze_state->y] != 1) {
            actions.push_back(std::make_shared<Action>("Up", 1, state, create_state(maze_state->x - 1, maze_state->y)));
        }
        if (maze_state->x < maze_state->maze.size() - 1 && maze_state->maze[maze_state->x + 1][maze_state->y] != 1) {
            actions.push_back(std::make_shared<Action>("Down", 1, state, create_state(maze_state->x + 1, maze_state->y)));
        }
        if (maze_state->y > 0 && maze_state->maze[maze_state->x][maze_state->y - 1] != 1) {
            actions.push_back(std::make_shared<Action>("Left", 1, state, create_state(maze_state->x, maze_state->y - 1)));
        }
        if (maze_state->y < maze_state->maze[0].size() - 1 && maze_state->maze[maze_state->x][maze_state->y + 1] != 1) {
            actions.push_back(std::make_shared<Action>("Right", 1, state, create_state(maze_state->x, maze_state->y + 1)));
        }

        return actions;
    }



    double heuristic(State *state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state);
        return abs(maze_state->x - 3) + abs(maze_state->y - 3);
    }
};


#endif //SIMPLE_MAZE_H
