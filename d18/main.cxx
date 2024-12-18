#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <cstdio>
#include <set>
#include <functional>
#include <optional>
#include <algorithm>
#include <ranges>

using std::cout, std::endl;
namespace views = std::ranges::views;

constexpr const char content[] = {
    #embed "input.txt"
};

namespace std {
    template <typename T1, typename T2>
    struct hash<std::pair<T1, T2>> {
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
        }
    };
}

struct Byte {
    int num;
    std::pair<int,int> pos;

    bool operator<(const Byte& other) const {
        return num < other.num;
    }

    bool operator==(const Byte& other) const {
        return num == other.num;
    } 

    bool operator!=(const Byte& other) const {
        return num != other.num;
    }
};

bool out_of_bounds(int min, int max, const std::pair<int,int>& pos) {
    auto [x, y] = pos;
    return x < min || y < min || x >= max || y >= max;
}

template <typename Range>
std::optional<int> shortest_path(const std::vector<std::vector<char>>& grid,const Range& bytes, const std::pair<int,int>& start, const std::pair<int,int>& end) {
    std::queue<std::pair<std::pair<int,int>, int>> q;
    q.push({start, 0});
    std::unordered_set<std::pair<int,int>> visited = {start};
    while(!q.empty()) {
        auto [pos, steps] = q.front();
        auto [x, y] = pos;
        q.pop();
        if(pos == end) {
            return steps;
        }
        for(auto [dx, dy] : std::vector<std::pair<int,int>>{{0,1}, {0,-1}, {1,0}, {-1,0}}) {
            auto new_pos = std::make_pair(x + dx, y + dy);
            auto [nx, ny] = new_pos;
            auto corrupted = std::ranges::find_if(bytes, [nx, ny](const Byte& byte) {
                return byte.pos == std::make_pair(nx, ny);
            }) != bytes.end();

            if(out_of_bounds(0, grid.size(), new_pos) || visited.contains(new_pos) || corrupted) {
                continue;
            }
            visited.insert(new_pos);
            q.push({new_pos, steps + 1});
        }
    }
    return std::nullopt;
}


int part_1() {
    std::istringstream input_stream(content);
    std::string line;

    constexpr auto byte_falls{1024};
    constexpr auto N{71};
    std::vector<std::vector<char>> grid(N, std::vector<char>(N, '.'));
    std::set<Byte> bytes;

    int byte_num{0};
    while(std::getline(input_stream, line) && bytes.size() < byte_falls) {
        Byte byte{static_cast<int>(bytes.size()), {0, 0}};
        sscanf(line.c_str(), "%d,%d", &byte.pos.second, &byte.pos.first); 
        bytes.insert(byte);
    }   

    auto start = std::make_pair(0, 0);
    auto end = std::make_pair(N-1, N-1);
    return shortest_path(grid, bytes, start, end).value();
}

std::string part_2() {
    std::istringstream input_stream(content);
    std::string line;

    constexpr auto N{71};
    std::vector<std::vector<char>> grid(N, std::vector<char>(N, '.'));
    std::set<Byte> bytes;

    while(std::getline(input_stream, line)){
        auto byte = Byte{static_cast<int>(bytes.size()), {0, 0}};
        sscanf(line.c_str(), "%d,%d", &byte.pos.second, &byte.pos.first);
        bytes.insert(byte);
    }    

    auto start = std::make_pair(0, 0);
    auto end = std::make_pair(N-1, N-1);

    int left= 0 , right = static_cast<int>(bytes.size() - 1);
    int result = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        auto sub_bytes = bytes | views::take(mid + 1);
        if (shortest_path(grid, sub_bytes, start, end).has_value()) {
            left = mid + 1; 
        } else {
            result = mid;    
            right = mid - 1;
        }
    }

    auto byte = std::find_if(bytes.begin(), bytes.end(), [result](const Byte& byte) { return byte.num == result; }); 
    return std::to_string(byte->pos.second) + "," + std::to_string(byte->pos.first); 
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
