#include <iostream>
#include <sstream>
#include <vector>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

int count_xmas(const std::vector<std::string> &grid, int i, int j) {

    bool up{true}, down{true}, left{true}, right{true}, up_left{true}, up_right{true}, down_left{true}, down_right{true};
    for(auto d : {std::pair{1, 'M'}, std::pair{2, 'A'}, std::pair{3, 'S'}}) {
        if(up && (i - d.first < 0 || grid[i - d.first][j] != d.second)) {
            up = false;
        } 

        if(down && (i + d.first >= grid[i].size() || grid[i + d.first][j] != d.second)) {
            down = false;
        }

        if(right && (j + d.first >= grid[i].size() || grid[i][j + d.first] != d.second)) {
            right = false;
        }

        if(left && (j - d.first < 0 || grid[i][j - d.first] != d.second)) {
            left = false;
        }

        if(up_right && (i - d.first < 0 || j + d.first >= grid[i].size() || grid[i - d.first][j + d.first] != d.second)) {
            up_right = false;
        }

        if(up_left && (i - d.first < 0 || j - d.first < 0 || grid[i - d.first][j - d.first] != d.second)) {
            up_left = false;
        }

        if(down_right && (i + d.first >= grid[i].size() || j + d.first >= grid[i].size() || grid[i + d.first][j + d.first] != d.second)) {
            down_right = false;
        }

        if(down_left && (i + d.first >= grid[i].size() || j - d.first < 0 || grid[i + d.first][j - d.first] != d.second)) {
            down_left = false;
        }
    } 
    
    return up + down + left + right + up_left + up_right + down_left + down_right;    
}

void part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> grid;

    while (std::getline(input_stream, line)) {
        grid.push_back(line);
    }

    int count_acc{0};
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == 'X') {
               count_acc += count_xmas(grid, i, j); 
            } 
        }
    }

    cout << count_acc << endl;
}


bool is_mas_cross(const std::vector<std::string>& grid, int i, int j) {
    char up_right_letter, up_left_letter, down_right_letter, down_left_letter;
    if(i - 1 >= 0 && j + 1 < grid[i].size() && (grid[i - 1][j + 1] == 'M' || grid[i - 1][j + 1] == 'S')) {
        up_right_letter = grid[i - 1][j + 1];
        down_left_letter = up_right_letter == 'M' ? 'S' : 'M';
    } else {
        return false;
    }
    if(i - 1 >= 0 && j - 1 >= 0 && (grid[i - 1][j - 1] == 'M' || grid[i - 1][j - 1] == 'S')) {
        up_left_letter = grid[i - 1][j - 1];
        down_right_letter = up_left_letter == 'M' ? 'S' : 'M';
    } else {
        return false;
    }
    if(i + 1 >= grid[i].size() || j + 1 >= grid[i].size() || grid[i + 1][j + 1] != down_right_letter) {
        return false;
    }
    if(i + 1 >= grid[i].size() || j - 1 < 0 || grid[i + 1][j - 1] != down_left_letter) { 
        return false;
    }
    
    return true;
}

void part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::string> grid;

    while (std::getline(input_stream, line)) {
        grid.push_back(line);
    }

    int count_acc{0};
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            if(grid[i][j] == 'A') {
               count_acc += is_mas_cross(grid, i, j);
            } 
        }
    }

    cout << count_acc << endl;
} 

int main() {
    part_1();
    part_2();
    return 0;
}
