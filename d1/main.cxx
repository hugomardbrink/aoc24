#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

void part_1() {
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

    cout << diff_acc << endl;
}

void part_2() {
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

    cout << sim_acc << endl;
} 

int main() {
    part_1();
    part_2();
    return 0;
}
