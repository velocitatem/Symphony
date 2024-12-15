//
// Created by velocitatem on 12/15/24.
//

#include "search.h"
#include <iostream>



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
        for (auto &row : maze) {
            for (auto &cell : row) {
                if (cell == 0) {
                    std::cout << " ";
                } else if (cell == 1) {
                    std::cout << "#";
                } else if (cell == -1) {
                    std::cout << "X";
                }
            }
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
        delete initial_state_;
    }
    bool goal_test(State *state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state);
        return maze_state->maze[maze_state->x][maze_state->y] == -1;
    }
    std::vector<Action> actions(State *state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state);
        std::vector<Action> actions;
        if (maze_state->x > 0 && maze_state->maze[maze_state->x - 1][maze_state->y] != 1) {
            actions.emplace_back("Up", 1, maze_state, new MazeState(maze_state->maze, maze_state->x - 1, maze_state->y));
        }
        if (maze_state->x < maze_state->maze.size() - 1 && maze_state->maze[maze_state->x + 1][maze_state->y] != 1) {
            actions.emplace_back("Down", 1, maze_state, new MazeState(maze_state->maze, maze_state->x + 1, maze_state->y));
        }
        if (maze_state->y > 0 && maze_state->maze[maze_state->x][maze_state->y - 1] != 1) {
            actions.emplace_back("Left", 1, maze_state, new MazeState(maze_state->maze, maze_state->x, maze_state->y - 1));
        }
        if (maze_state->y < maze_state->maze[0].size() - 1 && maze_state->maze[maze_state->x][maze_state->y + 1] != 1) {
            actions.emplace_back("Right", 1, maze_state, new MazeState(maze_state->maze, maze_state->x, maze_state->y + 1));
        }
        return actions;
    }
    double heuristic(State *state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state);
        return abs(maze_state->x - 3) + abs(maze_state->y - 3);
    }
};


int main () {
    {
        VacuumCleaner vacuum_cleaner;
        Search *search = create_search(SearchAlgorithmIndex::BREADTH_FIRST_SEARCH, &vacuum_cleaner);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    }

    {
        MazeProblem maze_problem;
        Search *search = create_search(SearchAlgorithmIndex::BREADTH_FIRST_SEARCH, &maze_problem);
        auto node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            MazeState *maze_state = dynamic_cast<MazeState *>(node->state);
            maze_state->print();
            Solution solution(node.get());
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;
    }

    return 0;
}