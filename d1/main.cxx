#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

int part_1() {
    std::vector<int> left_vec, right_vec;
    std::istringstream iss{content}; 

    int l_id, r_id;
    while (iss >> l_id >> r_id) {
        left_vec.emplace_back(l_id);
        right_vec.emplace_back(r_id);
    }
    std::sort(left_vec.begin(), left_vec.end());
    std::sort(right_vec.begin(), right_vec.end());

    auto diff_acc{0};
    for (auto i{0}; i < left_vec.size(); i++) {
        diff_acc += std::abs(left_vec[i] - right_vec[i]);
    }

    return diff_acc;
}

int part_2() {
    std::vector<int> left_vec;
    std::unordered_map<int, int> right_map;
    std::istringstream iss{content}; 

    int l_id, r_id;
    while (iss >> l_id >> r_id) {
        left_vec.emplace_back(l_id);
        right_map[r_id]++;
    }

    auto sim_acc{0};
    for (auto left_num : left_vec) {
        sim_acc += right_map[left_num] * left_num;
    }
    
    return sim_acc;
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
