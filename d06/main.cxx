#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <ranges> 
#include <unordered_set>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
}; 

struct State {
    int x;
    int y;
    Direction direction;

    bool operator==(const State& other) const {
        return x == other.x && y == other.y && direction == other.direction;
    }
};

namespace std {
    template <>
    struct hash<State> {
        std::size_t operator()(const State& s) const {
            return std::hash<int>{}(s.x) ^ std::hash<int>{}(s.y) ^ std::hash<int>{}(s.direction);
        }
    };
}

bool position_in_bounds(const std::vector<std::string>& path_map, int x, int y) {
    return x >= 0 && y >= 0 && x < path_map.size() && y < path_map[0].size();
}

int part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> path_map;

    int row{0};
    State state;
    while (std::getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }        
        path_map.emplace_back(line); 
        if(line.find('^') != std::string::npos) {
            state.x = row;
            state.y = line.find('^');
            state.direction = UP;
        }
        row++;
    }

    auto unique_acc{0};
    while (position_in_bounds(path_map, state.x, state.y)) {
        int walk_x{state.x};
        int walk_y{state.y};

        switch(state.direction) {
            case UP:
                walk_x--;
                break;
            case RIGHT:
                walk_y++;
                break;
            case DOWN:
                walk_x++;
                break;
            case LEFT:
                walk_y--;
                break;
            default:
                break;
         }

        if(!position_in_bounds(path_map, walk_x, walk_y)) {
            break;
        } else if(path_map[walk_x][walk_y] == '#') {
            state.direction = static_cast<Direction>((state.direction + 1) % 4);
        } else {
            state.x = walk_x;
            state.y = walk_y;

            if(path_map[state.x][state.y] != 'X') {
                unique_acc++;
                path_map[state.x][state.y] = 'X';
            } 
        }
    }
    return unique_acc;
}

bool loop_detected(std::vector<std::string>& path_map, State start_state) {
    std::unordered_set<State> visited_states;

    State state = start_state;
    while(position_in_bounds(path_map, state.x, state.y)) {
        int walk_x{state.x};
        int walk_y{state.y};

        switch(state.direction) {
            case UP:
                walk_x--;
                break;
            case RIGHT:
                walk_y++;
                break;
            case DOWN:
                walk_x++;
                break;
            case LEFT:
                walk_y--;
                break;
            default:
                break;
         }

        if(!position_in_bounds(path_map, walk_x, walk_y)) {
            return false;
        } else if(path_map[walk_x][walk_y] == '#') {
            state.direction = static_cast<Direction>((state.direction + 1) % 4);
        } else {
            state.x = walk_x;
            state.y = walk_y;
            if(visited_states.find(state) != visited_states.end()) {
                return true;
            } else {
                visited_states.insert(state);
            }
        }
    }

    return false;
}


int part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> path_map;
    std::unordered_set<State> obstacles;

    int row{0};
    State state;
    while (std::getline(input_stream, line)) {
        if(line.empty()) continue;

        path_map.emplace_back(line); 
        if(line.find('^') != std::string::npos) {
            state.x = row;
            state.y = line.find('^');
            state.direction = UP;
        }
        row++;
    }
    State start_state = state;
    while (position_in_bounds(path_map, state.x, state.y)) {
        int walk_x{state.x};
        int walk_y{state.y};

        switch(state.direction) {
            case UP:
                walk_x--;
                break;
            case RIGHT:
                walk_y++;
                break;
            case DOWN:
                walk_x++;
                break;
            case LEFT:
                walk_y--;
                break;
            default:
                break;
         }

        if(!position_in_bounds(path_map, walk_x, walk_y)) {
            break;
        } else if(path_map[walk_x][walk_y] == '#') {
            state.direction = static_cast<Direction>((state.direction + 1) % 4);
        } else {
            if(!(start_state.x == walk_x && walk_y == start_state.y)) {
                path_map[walk_x][walk_y] = '#';
                if(loop_detected(path_map, start_state) && obstacles.find({walk_x, walk_y}) == obstacles.end()) {
                    obstacles.insert({walk_x, walk_y});
                } 
                path_map[walk_x][walk_y] = '.';
            }
            state.x = walk_x;
            state.y = walk_y;
        }

    }

    return obstacles.size();
}

int main() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto p1_val = part_1();
    std::chrono::steady_clock::time_point middle = std::chrono::steady_clock::now();
    auto p2_val = part_2();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << "Part 1: " << p1_val << " (" << std::chrono::duration_cast<std::chrono::microseconds>(middle - begin).count() << "µs)" << endl;  
    cout << "Part 2: " << p2_val << " (" << std::chrono::duration_cast<std::chrono::microseconds>(end - middle).count() << "µs)" << endl;
    return 0;
}
