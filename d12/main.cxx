#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ranges>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct SidePosition {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct Region {
    char type;
    std::vector<Position> positions;
    int perimeter;
};

struct SideRegion {
    char type;
    std::vector<SidePosition> positions;
    int corners;
};

namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
        }
    };
}

void find_region(std::vector<std::string>& garden, std::vector<Region>& regions, std::unordered_set<std::pair<int, int>>& visited, Position p, Region& curr_region, char target) {
    if(p.x < 0 || p.x >= garden.size() || p.y < 0 || p.y >= garden[p.x].size()) {
        return;
    }
    
    for(auto [i, j] : {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
        if(p.x + i >= 0 && p.x + i < garden.size() && p.y + j >= 0 && p.y + j < garden[p.x].size()) {
            if(garden[p.x + i][p.y + j] == target && !visited.contains({p.x + i, p.y + j})) {
                visited.insert({p.x + i, p.y + j});
                find_region(garden, regions, visited, {p.x + i, p.y + j}, curr_region, target);
            } else if(garden[p.x + i][p.y + j] != target){
                curr_region.perimeter++;
            }
        } else {
            curr_region.perimeter++;
        }
    }

    curr_region.positions.push_back(p);
}

int part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> garden = {};
    std::vector<Region> regions = {};
    std::unordered_set<std::pair<int,int>> visited = {};

    while(getline(input_stream, line)) {
        garden.emplace_back(line);
    }

    int id = 0; 
    for(int x = 0; x < garden.size(); x++) {
        for(int y = 0; y < garden[x].size(); y++) {
            if(!visited.contains({x, y})) {
                auto curr_region = Region{garden[x][y], {}, 0};
                visited.insert({x, y});
                find_region(garden, regions, visited, {x, y}, curr_region, garden[x][y]);
                regions.push_back(curr_region);
            }
        }
    }

    int total = 0;
    for(auto& region : regions) {
        total += region.perimeter * region.positions.size();
    }


    return total;
}
    
void find_corners(std::vector<std::string>& garden, std::vector<SideRegion>& regions, std::unordered_set<std::pair<int, int>>& visited, SidePosition p, SideRegion& curr_region, char target) {
    if(p.x < 0 || p.x >= garden.size() || p.y < 0 || p.y >= garden[p.x].size()) {
        return;
    }

    int edge_count = 0;
    for(auto [i, j] : {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
        if(p.x + i >= 0 && p.x + i < garden.size() && p.y + j >= 0 && p.y + j < garden[p.x].size()) {
            if(garden[p.x + i][p.y + j] != target) {
                edge_count++;
            }
        } else {
            edge_count++;
        }
    }
    if(edge_count == 0) {
        for(auto [i, j] : {std::pair{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}) {
            if(p.x + i >= 0 && p.x + i < garden.size() && p.y + j >= 0 && p.y + j < garden[p.x].size()) {
                if(garden[p.x + i][p.y + j] != target) {
                    edge_count++;
                }
            } else {
                edge_count++;
            }
        }

        if(edge_count == 1) {
            curr_region.corners++;
        }
    } else if(edge_count == 2) {
        curr_region.corners++;
    }

    

    for(auto [i, j] : {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}}) {
        if(p.x + i >= 0 && p.x + i < garden.size() && p.y + j >= 0 && p.y + j < garden[p.x].size()) {
            if(garden[p.x + i][p.y + j] == target && !visited.contains({p.x + i, p.y + j})) {
                visited.insert({p.x + i, p.y + j});
                find_corners(garden, regions, visited, {p.x + i, p.y + j}, curr_region, target);
            } else if(garden[p.x + i][p.y + j] != target){
            }
        } else {
        }
    }

    curr_region.positions.push_back(p);
}

int part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> garden = {}, scaled_garden = {};
    std::vector<SideRegion> regions = {};
    std::unordered_set<std::pair<int,int>> visited = {};

    while(getline(input_stream, line)) {
        garden.emplace_back(line);
    }

    for(auto& row : garden) {
        std::string s{""};
        for(int i = 0; i < row.size(); i++) {
            for(auto _ : std::views::iota(0, 3)) {
                s.push_back(row[i]);
            }
        }
        for(auto _ : std::views::iota(0, 3)) {
            scaled_garden.push_back(s);
        }
    }

    int id = 0; 
    for(int x = 0; x < scaled_garden.size(); x++) {
        for(int y = 0; y < scaled_garden[x].size(); y++) {
            if(!visited.contains({x, y})) {
                auto curr_region = SideRegion{scaled_garden[x][y], {}, 0};
                visited.insert({x, y});
                find_corners(scaled_garden, regions, visited, {x, y}, curr_region, scaled_garden[x][y]);
                regions.push_back(curr_region);
            }
        }
    }

    int total = 0;
    for(auto& region : regions) {
        total += region.corners * (region.positions.size()/9) ;
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
