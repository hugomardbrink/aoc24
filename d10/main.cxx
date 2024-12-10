#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <chrono>
#include <iostream>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

using Position = std::pair<int, int>;

bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& p) const {
            return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
        }
    };
}

int path_count(std::vector<std::string>& grid, Position p, std::unordered_set<Position>& seen) {
    constexpr auto target = '9';

    if (seen.contains(p)) {
        return 0;
    } else {
        seen.insert(p);
    }

    if (grid[p.first][p.second] == target) {
        return 1;
    }

    int paths = 0;
    for(auto [i, j] : {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
        const int nexti = p.first + i;
        const int nextj = p.second + j;
        if (nexti < 0 || nexti >= grid.size() || nextj < 0 || nextj >= grid[nexti].size()) {
            continue;
        }
        if (grid[nexti][nextj] == grid[p.first][p.second] + 1) {
            paths += path_count(grid, {nexti, nextj}, seen);
        }
    }

    return paths;
}

int part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> grid = {};
    while(std::getline(input_stream, line)) {
        grid.push_back(line);
    }

    int tailheads = 0;
    for(int i = 0; i < grid.size(); ++i) {
        for(int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == '0') {
                std::unordered_set<Position> seen;
                tailheads += path_count(grid, {i, j}, seen);
            }
        }
    }

    return tailheads;
}

int all_path_count(std::vector<std::string>& grid, Position p) {
    constexpr auto target = '9';
    
    if (grid[p.first][p.second] == target) {
        return 1;
    }

    int paths = 0;
    for(auto [i, j] : {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
        const int nexti = p.first + i;
        const int nextj = p.second + j;
        if (nexti < 0 || nexti >= grid.size() || nextj < 0 || nextj >= grid[nexti].size()) {
            continue;
        }
        if (grid[nexti][nextj] == grid[p.first][p.second] + 1) {
            paths += all_path_count(grid, {nexti, nextj});
        }
    }

    return paths;
}

int part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> grid = {};
    while(std::getline(input_stream, line)) {
        grid.push_back(line);
    }

    int tailheads = 0;
    for(int i = 0; i < grid.size(); ++i) {
        for(int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == '0') {
                tailheads += all_path_count(grid, {i, j});
            }
        }
    }

    return tailheads;
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
