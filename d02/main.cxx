#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <omp.h>
using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

enum class Direction {
    Inc,
    Dec
};

bool safe(const std::vector<int>& level) {
  bool inc{true}, dec{true};
  auto sorted_level = level;
  for(int i = 1; i < level.size(); i++) {
    if (level[i] <= level[i-1]) {
      inc = false;
    } 
    if (level[i] >= level[i-1]) {
      dec = false;
    }
  }
  if (!inc && !dec) {
    return false;
  }
  if (dec) {
    sorted_level = std::vector<int>(sorted_level.rbegin(), sorted_level.rend());
  }

  for(int i = 1; i < sorted_level.size(); i++) {
    if (sorted_level[i] - sorted_level[i-1] > 3) {
      return false;
    }
  }
  return true;
}

int part_1() {
    std::istringstream iss{content}; 
    std::string line;
    int correct_acc{0};
    std::vector<std::vector<int>> levels = {};

    while(getline(iss, line)) {
        std::istringstream line_ss{line};
        std::vector<int> level = {};

        int cur_level;
        while(line_ss >> cur_level) {
            level.emplace_back(cur_level);
        }
        levels.emplace_back(level);
    }

    for(auto& level : levels) {
        if(safe(level)) {
            correct_acc++;
        }
    }
    return correct_acc;
}

bool safe_dampener(const std::vector<int>& level) {
    if(safe(level)) {
        return true;
    }

    for(int i = 0; i < level.size(); i++) {
        auto modified = level;
        modified.erase(modified.begin() + i);
        if (safe(modified)) {
            return true;
        }
    }
    return false;
}

int part_2() {
    std::istringstream iss{content}; 
    std::string line;
    int correct_acc{0};
    std::vector<std::vector<int>> levels = {};

    while(getline(iss, line)) {
        std::istringstream line_ss{line};
        std::vector<int> level = {};

        int cur_level;
        while(line_ss >> cur_level) {
            level.emplace_back(cur_level);
        }
        levels.emplace_back(level);
    }

    for(auto& level : levels) {
        if(safe_dampener(level)) {
            correct_acc++;
        }
    }
    return correct_acc;
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
