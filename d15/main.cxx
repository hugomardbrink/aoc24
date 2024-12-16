#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_map>
#include <numeric>

using std::cout, std::endl, std::pair;

namespace views = std::ranges::views;

constexpr const char content[] = {
    #embed "input.txt"
};

enum class Direction : char {
    UP = '^',
    DOWN = 'v',
    LEFT = '<',
    RIGHT = '>',
};

enum class TileType : char {
    ROBOT = '@',
    WALL = '#',
    BOX = 'O',
    EMPTY = '.',
};

namespace std {
    template<>
    struct hash<std::pair<int,int>> {
        std::size_t operator()(const std::pair<int,int>& k) const {
            return std::hash<int>()(k.first) ^ std::hash<int>()(k.second);
        }
    };
}

bool move_tile(std::pair<int,int> pos, Direction dir, std::unordered_map<std::pair<int,int>, TileType>& map, TileType type) {
    int h = Direction::LEFT == dir ? -1 : Direction::RIGHT == dir ? 1 : 0;
    int v = Direction::UP == dir ? -1 : Direction::DOWN == dir ? 1 : 0; 

    if(map[{pos.first + h, pos.second + v}] == TileType::EMPTY) {
        map[{pos.first + h, pos.second + v}] = type;
        map[{pos.first, pos.second}] = TileType::EMPTY;
        return true;
    } else if(map[{pos.first + h, pos.second + v}] == TileType::BOX) {
        if(move_tile({pos.first + h, pos.second + v}, dir, map, TileType::BOX)) {
            map[{pos.first + h, pos.second + v}] = type;
            map[{pos.first, pos.second}] = TileType::EMPTY;
            return true;
        } else {
            return false;
        }
    } else if(map[{pos.first + h , pos.second + v}] == TileType::WALL) {
        return false;
    }
    return false;
}

int64_t part_1() {
    std::istringstream input_stream(content);
    std::string line;

    std::unordered_map<std::pair<int,int>, TileType> map = {};

    int y = 0, x = 0;
    std::pair<int, int> robot_pos;
    while(getline(input_stream, line)) {
        if(line == "") break;

        x = 0;
        for(auto c : line) {
            switch (c) {
                case '#':
                    map[{x, y}] = TileType::WALL;
                    break;
                case '@':
                    robot_pos = {x, y};
                    map[{x, y}] = TileType::ROBOT;
                    break;
                case 'O':
                    map[{x, y}] = TileType::BOX;
                    break;
                default:
                    map[{x, y}] = TileType::EMPTY;
                    break;
            }
            x++;
        }
        y++;
    }

    std::vector<Direction> movements = {};
    while(getline(input_stream, line)) {
        if(line.empty()) break;
        for(auto c : line) {
            movements.push_back(static_cast<Direction>(c));
        }
    }

    for(auto m : movements) {
       if(move_tile(robot_pos, m, map, TileType::ROBOT)) {
            int h = Direction::LEFT == m ? -1 : Direction::RIGHT == m ? 1 : 0;
            int v = Direction::UP == m ? -1 : Direction::DOWN == m ? 1 : 0;
            robot_pos.first += h;
            robot_pos.second += v;
       }
    }

    auto total = 0;
    for(auto [k, v] : map) {
        if(v == TileType::BOX) {
            total += 100 * k.second + k.first;
        }
    }

    return total;
}

enum class ScaledTileType {
    ROBOT,
    WALL,
    LBOX,
    RBOX,
    EMPTY,
};

bool is_vertical_box_free(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    int v = Direction::UP == dir ? -1 : Direction::DOWN == dir ? 1 : 0;

    std::pair<int, int> lft_pos, right_pos;
    if(type == ScaledTileType::LBOX) {
        lft_pos = {pos.first , pos.second};
        right_pos = {pos.first + 1, pos.second};
    } else {
        lft_pos = {pos.first - 1, pos.second};
        right_pos = {pos.first, pos.second};
    }

    auto next_right = map[{right_pos.first, right_pos.second + v}];
    auto next_left = map[{lft_pos.first, lft_pos.second + v}];

    if(next_left == ScaledTileType::EMPTY && next_right == ScaledTileType::EMPTY) {
        return true;
    } else if(next_left == ScaledTileType::WALL || next_right == ScaledTileType::WALL) {
        return false;
    }
    bool box_free = true;
    if(next_left == ScaledTileType::LBOX || next_left == ScaledTileType::RBOX) {
        auto box_type = next_left;
        box_free &= is_vertical_box_free({lft_pos.first, lft_pos.second + v}, box_type, map, dir);
    }
    if(next_right == ScaledTileType::LBOX || next_right == ScaledTileType::RBOX) {
        auto box_type = next_right; 
        box_free &= is_vertical_box_free({right_pos.first, right_pos.second + v}, box_type, map, dir);
    }
    return box_free;
}

bool is_horizontal_box_free(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    int h = Direction::LEFT == dir ? -1 : Direction::RIGHT == dir ? 1 : 0;

    auto next = map[{pos.first + h, pos.second}];
    
    if(next == ScaledTileType::EMPTY) {
        return true;
    } else if (next == ScaledTileType::RBOX || next == ScaledTileType::LBOX) {
        return is_horizontal_box_free({pos.first + h, pos.second}, next, map, dir);
    } else {
        return false;
    }
}

bool is_box_free(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    switch (dir) {
        case Direction::UP:
        case Direction::DOWN:
            return is_vertical_box_free(pos, type, map, dir);
        case Direction::LEFT:
        case Direction::RIGHT:
            return is_horizontal_box_free(pos, type, map, dir);
    }
}

bool move_vertical_tile(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    int v = Direction::UP == dir ? -1 : Direction::DOWN == dir ? 1 : 0;

    auto next_pos = std::pair<int,int>{pos.first, pos.second + v};
    auto next = map[{pos.first, pos.second + v}];
    if(next == ScaledTileType::EMPTY) {
        map[next_pos] = type;
        map[pos] = ScaledTileType::EMPTY;
        return true;
    } else if(next == ScaledTileType::LBOX) {
        if(is_vertical_box_free(next_pos, ScaledTileType::LBOX, map, dir)) {
            move_vertical_tile(next_pos, ScaledTileType::LBOX, map, dir);
            move_vertical_tile({pos.first + 1, pos.second + v}, ScaledTileType::RBOX, map, dir);
            map[next_pos] = type;
            map[pos] = ScaledTileType::EMPTY;
            return true;
        } else {
            return false;
        }
    } else if(next == ScaledTileType::RBOX) {
        if(is_vertical_box_free(next_pos, ScaledTileType::RBOX, map, dir)) {
            move_vertical_tile(next_pos, ScaledTileType::RBOX, map, dir);
            move_vertical_tile({pos.first - 1, pos.second + v}, ScaledTileType::LBOX, map, dir);
            map[next_pos] = type;
            map[pos] = ScaledTileType::EMPTY;
            return true;
        } else {
            return false;
        }
    } else if(next == ScaledTileType::WALL) {
        return false;
    }

    return false;
}

bool move_horizontal_tile(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    int h = Direction::LEFT == dir ? -1 : Direction::RIGHT == dir ? 1 : 0;

    auto next_pos = std::pair<int,int>{pos.first + h, pos.second};
    auto next = map[{pos.first + h, pos.second}];
    if(next == ScaledTileType::EMPTY) {
        map[next_pos] = type;
        map[pos] = ScaledTileType::EMPTY;
        return true;
    } else if(next == ScaledTileType::LBOX || next == ScaledTileType::RBOX) {
        if(is_horizontal_box_free(next_pos, next, map, dir)) {
            move_horizontal_tile(next_pos, next, map, dir);
            map[next_pos] = type;
            map[pos] = ScaledTileType::EMPTY;
            return true;
        } else {
            return false;
        }
    } else if(next == ScaledTileType::WALL) {
        return false;
    }

    return false;
}

bool move_scaled_tile(std::pair<int,int> pos, ScaledTileType type, std::unordered_map<std::pair<int,int>, ScaledTileType>& map, Direction dir) {
    switch (dir) {
        case Direction::UP:
        case Direction::DOWN:
            return move_vertical_tile(pos, type, map, dir);
        case Direction::LEFT:
        case Direction::RIGHT:
            return move_horizontal_tile(pos, type, map, dir);
    }
       
    return false;
}

int64_t part_2() {
    std::istringstream input_stream(content);
    std::string line;

    std::unordered_map<std::pair<int,int>, ScaledTileType> map = {};

    int y = 0, x = 0;
    std::pair<int, int> robot_pos;
    while(getline(input_stream, line)) {
        if(line == "") break;

        x = 0; 
        for(auto c : line) {
            switch(c) {
                case '#':
                    map[{x, y}] = ScaledTileType::WALL;
                    map[{x+1, y}] = ScaledTileType::WALL;
                    break;
                case '@':
                    robot_pos = {x, y};
                    map[{x, y}] = ScaledTileType::ROBOT;
                    map[{x+1, y}] = ScaledTileType::EMPTY;
                    break;
                case 'O':
                    map[{x, y}] = ScaledTileType::LBOX;
                    map[{x+1, y}] = ScaledTileType::RBOX;
                    break;
                default:
                    map[{x, y}] = ScaledTileType::EMPTY;
                    map[{x+1, y}] = ScaledTileType::EMPTY;
                    break;
            }
            x += 2;
        }
        y++;
    }

    std::vector<Direction> movements = {};
    while(getline(input_stream, line)) {
        if(line.empty()) break;
        for(auto c : line) {
            movements.push_back(static_cast<Direction>(c));
        }
    }

    for(auto m : movements) {
       if(move_scaled_tile(robot_pos, ScaledTileType::ROBOT, map, m)) {
           int v = Direction::UP == m ? -1 : Direction::DOWN == m ? 1 : 0;
           int h = Direction::LEFT == m ? -1 : Direction::RIGHT == m ? 1 : 0;
           robot_pos.first += h;
           robot_pos.second += v;
       }
    } 

    auto total = 0;
    for(auto [k, v] : map) {
        if(v == ScaledTileType::LBOX) {
            total += 100 * k.second + k.first;
        }
    }

    return total;
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
