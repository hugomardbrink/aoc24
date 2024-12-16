#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_set>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

enum class Direction { 
    N,
    S,
    E,
    W
};

struct Position {
    size_t x;
    size_t y;
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& p) const {
            return std::hash<size_t>{}(p.x) ^ std::hash<size_t>{}(p.y);
        }
    };
}

struct Node {
    int cost;
    Position pos;
    int dir;
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

std::optional<int> find_shortest_path(Position start, Position end, std::vector<std::string>& grid) {
int rows = grid.size(), cols = grid[0].size();
    std::vector<std::vector<std::vector<size_t>>> dist(
        rows, 
        std::vector<std::vector<size_t>>(
            cols, 
            std::vector<size_t>(4, std::numeric_limits<size_t>::max())
        )
    );
    std::array<std::pair<size_t, size_t>, 4> dirs = {std::pair<size_t, size_t>{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    
    for (int d = 0; d < 4; ++d) {
        dist[start.y][start.x][d] = 0;
        pq.push({0, {start.y, start.x}, d});
    }
    
    while (!pq.empty()) {
        auto [cost, pos, curr_dir] = pq.top();
        auto [y, x] = pos;
        pq.pop();
        
        if (y == end.y && x == end.x) return cost;
        
        if (cost > dist[y][x][curr_dir]) continue;
        
        for (int d = 0; d < 4; ++d) {
            auto ny = y + dirs[d].first;
            auto nx = x + dirs[d].second;
            
            if (grid[ny][nx] == '#') 
                continue;
            
            auto new_cost = cost + 1;
            if (curr_dir != d) 
                new_cost += 1000;
            
            if (new_cost < dist[ny][nx][d]) {
                dist[ny][nx][d] = new_cost;
                pq.push({new_cost, {ny, nx}, d});
            }
        }
    }

    return std::nullopt; 
}

int part_1() {
    std::istringstream input_stream(content);
    std::string line;

    Position start;
    Position end;
    std::vector<std::string> grid = {};

    size_t line_num = 0;
    while(std::getline(input_stream, line)) {
        if(line.contains("S")) start = {line.find("S"), line_num};
        if(line.contains("E")) end = {line.find("E"), line_num};
        
        grid.push_back(line);
        line_num++;
    }

    auto result = find_shortest_path(start, end, grid);
    if(result.has_value()) {
        return result.value();
    } else {
        return -1;
    }
}

struct SittingNode {
    int cost;
    std::unordered_set<Position> path = {};
    Position pos;
    int dir;

    bool operator>(const SittingNode& other) const {
        return cost > other.cost;
    }
};

std::optional<int> find_best_sitting_place(Position start, Position end, std::vector<std::string>& grid) {
    std::unordered_set<Position> all_short_paths = {};
    int rows = grid.size(), cols = grid[0].size();
    std::vector<std::vector<std::vector<size_t>>> dist(
        rows, 
        std::vector<std::vector<size_t>>(
            cols, 
            std::vector<size_t>(4, std::numeric_limits<size_t>::max())
        )
    );
    std::array<std::pair<size_t, size_t>, 4> dirs = {std::pair<size_t, size_t>{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    std::priority_queue<SittingNode, std::vector<SittingNode>, std::greater<SittingNode>> pq;
    
    for (int d = 0; d < 4; ++d) {
        dist[start.y][start.x][d] = 0;
        std::unordered_set<Position> path = {start};
        pq.push({0, path, {start.y, start.x }, d});
    }
    
    while (!pq.empty()) {
        auto [cost, path, pos, curr_dir] = pq.top();
        auto [y, x] = pos;
        pq.pop();
        
        if (y == end.y && x == end.x) {
            path.insert({y, x});
            all_short_paths.merge(path);
        }

        if (cost > dist[y][x][curr_dir]) continue;
        
        for (int d = 0; d < 4; ++d) {
            auto ny = y + dirs[d].first;
            auto nx = x + dirs[d].second;
            
            if (grid[ny][nx] == '#') 
                continue;
            
            auto new_cost = cost + 1;
            auto new_path = path;
            new_path.insert({ny, nx});
            if (curr_dir != d) 
                new_cost += 1000;
            
            if (new_cost <= dist[ny][nx][d]) {
                dist[ny][nx][d] = new_cost;
                pq.push({new_cost, new_path, {ny, nx}, d});
            }
        }
    }
    return all_short_paths.size();
}

int part_2() {
    std::istringstream input_stream(content);
    std::string line;

    Position start;
    Position end;
    std::vector<std::string> grid = {};

    size_t line_num = 0;
    while(std::getline(input_stream, line)) {
        if(line.contains("S")) start = {line.find("S"), line_num};
        if(line.contains("E")) end = {line.find("E"), line_num};
        
        grid.push_back(line);
        line_num++;
    }

    auto result = find_best_sitting_place(start, end, grid);
    if(result.has_value()) {
        return result.value();
    } else {
        return -1;
    }
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

