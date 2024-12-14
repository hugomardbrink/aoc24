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

namespace std {
    template<>
    struct hash<pair<int, int>> {
        std::size_t operator()(const pair<int, int>& p) const {
            return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
        }
    };
}
constexpr auto height{103};
constexpr auto width{101};

struct Robot {
    pair<int, int> pos;
    pair<int, int> velocity;
};

void move_robot(Robot& r, int time) {
    r.pos.first = (r.pos.first + time * r.velocity.first) % width;
    r.pos.second = (r.pos.second + time * r.velocity.second) % height;

    if(r.pos.first < 0) {
        r.pos.first = width + r.pos.first;
    }

    if(r.pos.second < 0) {
        r.pos.second = height + r.pos.second;
    } 
}

std::optional<int> get_quadrant(const pair<int, int>& p) {
    if(p.first == width / 2 || p.second == height / 2) {
        return std::nullopt;
    }

    if(p.first <= width / 2 && p.second <= height / 2) {
        return 0;
    } else if(p.first > width / 2 && p.second <= height / 2) {
        return 1;
    } else if(p.first <= width / 2 && p.second > height / 2) {
        return 2;
    } else {
        return 3;
    }

}

int64_t part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<Robot> robots = {};

    while(getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::replace_if(line.begin(), line.end(), [](char c) { return !std::isdigit(c) && c != '-'; }, ' ');
        Robot r;
        std::istringstream iss(line);
        iss >> r.pos.first >> r.pos.second >> r.velocity.first >> r.velocity.second;
        robots.push_back(r);
    } 

    std::vector<int> quadrant_count(4, 0);
    constexpr auto time{100};
    for(auto& r : robots) {
        move_robot(r, time);
        auto q = get_quadrant(r.pos);
        if(q.has_value()) {
            quadrant_count[q.value()]++;
        }
    }

    auto safety_factor = std::accumulate(quadrant_count.begin(), quadrant_count.end(), 1, std::multiplies<int>());
    return safety_factor;
}
 
void print_robots(const std::vector<Robot>& robots) {
    std::unordered_map<pair<int, int>, char> grid;
    for(auto& r : robots) {
        grid[r.pos] = '#';
    }

    for(auto i = 0; i < height; i++) {
        for(auto j = 0; j < width; j++) {
            if(grid.contains({j, i})) {
                cout << grid[{j, i}];
            } else {
                cout << '.';
            }
        }
        cout << endl;
    }
}

int64_t part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<Robot> robots = {};

    while(getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::replace_if(line.begin(), line.end(), [](char c) { return !std::isdigit(c) && c != '-'; }, ' ');
        Robot r;
        std::istringstream iss(line);
        iss >> r.pos.first >> r.pos.second >> r.velocity.first >> r.velocity.second;
        robots.push_back(r);
    } 

    auto time_to_tree = 0; // (ttt)
    while(true) {
        time_to_tree++;
        for(auto& r : robots) {
            move_robot(r, 1);
        }
       
        auto count = 0;
        for(auto& r : robots) {
            count = std::count_if(robots.begin(), robots.end(), [&r](const Robot& r2) {
                return std::abs(r.pos.first - r2.pos.first) <= 2 && std::abs(r.pos.second - r2.pos.second) <= 2;
            });
        }
    
        // for my input 20 was the magic number 
        if(count >= 20) {
            print_robots(robots);
            break;
        }

    }
    
    return time_to_tree;
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
