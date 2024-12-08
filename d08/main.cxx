#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <unordered_set>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

bool position_in_bounds(int x, int y, int max_x, int max_y) {
    return x >= 0 && x <= max_x && y >= 0 && y <= max_y;
}

namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const {
            auto h1 = std::hash<int>{}(p.first);
            auto h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 << 1); 
        }
    };
}

void get_antinodes(std::unordered_set<std::pair<int, int>>& antinodes, std::pair<int, int> a1, std::pair<int, int> a2, int max_x, int max_y) { 
    auto [x1, y1] = a1;
    auto [x2, y2] = a2;
    auto dx = x2 - x1; 
    auto dy = y2 - y1;

    auto gcd = std::gcd(dx, dy);
    dx /= gcd;
    dy /= gcd;

    auto x_fst = x1 - dx;
    auto y_fst = y1 - dy;
    if(position_in_bounds(x_fst, y_fst, max_x, max_y)) {
        antinodes.insert({x_fst, y_fst});
    }

    auto x_snd = x2 + dx;
    auto y_snd = y2 + dy;
    if(position_in_bounds(x_snd, y_snd, max_x, max_y)) {
        antinodes.insert({x_snd, y_snd});
    }
}

int part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::unordered_map<char, std::vector<std::pair<int, int>>> frequencies_map;

    auto row{0}, col{0};
    while (std::getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::istringstream line_stream(line);
        char frequency;
        col = 0;
        while (line_stream >> frequency) {
            if(frequency != '.') {
                if(frequencies_map.find(frequency) == frequencies_map.end()) {
                    frequencies_map[frequency] = {};
                }
                frequencies_map[frequency].emplace_back(row, col);
            }
            col++;
        }
        row++;
    } 
    int max_x = row - 1, max_y = col - 1;

    std::unordered_set<std::pair<int, int>> antinodes = {};
    for(auto &[freq, positions] : frequencies_map) {
        for(auto &pos_1 : positions) {
            for(auto &pos_2 : positions) {
                if(pos_1 != pos_2) {
                    get_antinodes(antinodes, pos_1, pos_2, max_x, max_y);
                }
            }
        }
    }

    return antinodes.size();
}

void get_all_antinodes(std::unordered_set<std::pair<int, int>>& antinodes, std::pair<int, int> a1, std::pair<int, int> a2, int max_x, int max_y) { 
    int num_antinodes{0};

    auto [x1, y1] = a1;
    auto [x2, y2] = a2;
    auto dx = x2 - x1; 
    auto dy = y2 - y1;

    auto gcd = std::gcd(dx, dy);
    dx /= gcd;
    dy /= gcd;

    auto x_fst = x1 - dx;
    auto y_fst = y1 - dy;
    while(position_in_bounds(x_fst, y_fst, max_x, max_y)) {
        antinodes.insert({x_fst, y_fst});
        x_fst -= dx;
        y_fst -= dy;
    }

    auto x_snd = x2 + dx;
    auto y_snd = y2 + dy;
    while(position_in_bounds(x_snd, y_snd, max_x, max_y)) {
        antinodes.insert({x_snd, y_snd});
        x_snd += dx;
        y_snd += dy;
    }
}

int part_2() {
    std::istringstream input_stream(content);
    std::string line;

    std::unordered_map<char, std::vector<std::pair<int, int>>> frequencies_map;
    std::unordered_set<std::pair<int, int>> antinodes = {};

    auto row{0}, col{0};
    while (std::getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::istringstream line_stream(line);
        char frequency;
        col = 0;
        while (line_stream >> frequency) {
            if(frequency != '.') {
                if(frequencies_map.find(frequency) == frequencies_map.end()) {
                    frequencies_map[frequency] = {};
                }
                frequencies_map[frequency].emplace_back(row, col);
                antinodes.insert({row, col});
            }
            col++;
        }
        row++;
    } 
    int max_x = row - 1, max_y = col - 1;

    for(auto &[freq, positions] : frequencies_map) {
        for(auto &pos_1 : positions) {
            for(auto &pos_2 : positions) {
                if(pos_1 != pos_2) {
                    get_all_antinodes(antinodes, pos_1, pos_2, max_x, max_y);
                }
            }
        }
    }

    return antinodes.size();
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
