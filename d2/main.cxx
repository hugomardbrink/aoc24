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

void part_1() {
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
    cout << correct_acc << endl;
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

void part_2() {
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
    cout << correct_acc << endl;
} 

int main() {
    part_1();
    part_2();
    return 0;
}
