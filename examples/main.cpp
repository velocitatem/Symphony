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
    }
    bool goal_test(State *state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state);
        /// nul.l check
        if (vacuum_state == nullptr) {
            return false;
        }
        return !vacuum_state->dirty0 && !vacuum_state->dirty1;
    }
    std::vector<Action> actions(std::shared_ptr<State> state) override {
        auto *vacuum_state = dynamic_cast<VacuumState *>(state.get());
        std::vector<Action> actions;

        // Define lambda to create a new VacuumState safely
        auto create_state = [&](int x, bool dirty0, bool dirty1) {
            return std::make_shared<VacuumState>(x, dirty0, dirty1);
        };

        // Check possible moves and construct actions
        if (vacuum_state->x == 0) {
            if (vacuum_state->dirty0) {
                actions.emplace_back("Suck", 1, std::make_shared<VacuumState>(*vacuum_state), create_state(0, false, vacuum_state->dirty1));
            } else {
                actions.emplace_back("Right", 1, std::make_shared<VacuumState>(*vacuum_state), create_state(1, vacuum_state->dirty0, vacuum_state->dirty1));
            }
        } else {
            if (vacuum_state->dirty1) {
                actions.emplace_back("Suck", 1, std::make_shared<VacuumState>(*vacuum_state), create_state(1, vacuum_state->dirty0, false));
            } else {
                actions.emplace_back("Left", 1, std::make_shared<VacuumState>(*vacuum_state), create_state(0, vacuum_state->dirty0, vacuum_state->dirty1));
            }
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
    std::vector<Action> actions(std::shared_ptr<State> state) override {
        auto *maze_state = dynamic_cast<MazeState *>(state.get());
        std::vector<Action> actions;

        // Define lambda to create a new MazeState safely
        auto create_state = [&](int x, int y) {
            return std::make_shared<MazeState>(maze_state->maze, x, y);
        };

        // Check possible moves and construct actions
        if (maze_state->x > 0 && maze_state->maze[maze_state->x - 1][maze_state->y] != 1) {
            actions.emplace_back("Up", 1, std::make_shared<MazeState>(*maze_state), create_state(maze_state->x - 1, maze_state->y));
        }
        if (maze_state->x < maze_state->maze.size() - 1 && maze_state->maze[maze_state->x + 1][maze_state->y] != 1) {
            actions.emplace_back("Down", 1, std::make_shared<MazeState>(*maze_state), create_state(maze_state->x + 1, maze_state->y));
        }
        if (maze_state->y > 0 && maze_state->maze[maze_state->x][maze_state->y - 1] != 1) {
            actions.emplace_back("Left", 1, std::make_shared<MazeState>(*maze_state), create_state(maze_state->x, maze_state->y - 1));
        }
        if (maze_state->y < maze_state->maze[0].size() - 1 && maze_state->maze[maze_state->x][maze_state->y + 1] != 1) {
            actions.emplace_back("Right", 1, std::make_shared<MazeState>(*maze_state), create_state(maze_state->x, maze_state->y + 1));
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
        std::shared_ptr<Node> node = search->search();

        if (node) {
            std::cout << "Solution found!" << std::endl;
            State *vacuum_state = node->state.get();
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
            State *maze_state = node->state.get();
            maze_state->print();
        } else {
            std::cout << "Solution not found!" << std::endl;
        }

        delete search;

    }

}