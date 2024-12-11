#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

uint64_t num_stones(std::vector<uint64_t>& numbers, uint64_t blinks) {
    std::unordered_map<uint64_t, uint64_t> nums = {};
    for(auto num : numbers) {
        nums[num] = 1;
    }

    for(int i{0}; i < blinks; i++) {
        std::unordered_map<uint64_t, uint64_t> new_nums = {};
        for(auto &[num, count] : nums) {
            if(num == 0) {
                new_nums[1] += count;
            } else if (std::to_string(num).length() % 2 == 0) {
                std::string num_str = std::to_string(num);
                uint64_t left = std::stoi(num_str.substr(0, num_str.length() / 2));
                uint64_t right = std::stoi(num_str.substr(num_str.length() / 2));
                new_nums[left] += count;
                new_nums[right] += count;
            } else {
                uint64_t new_num = num * 2024;
                new_nums[new_num] += count;
            }
        }
        nums = new_nums;
    }

    uint64_t total = 0;
    for(auto &[_, count] : nums) {
        total += count;
    }

    return total;
}

uint64_t part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<uint64_t> numbers = {};

    uint64_t curr_num;
    while(input_stream >> curr_num) {
        numbers.emplace_back(curr_num);
    }

    return num_stones(numbers, 25);
}
    
uint64_t part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<uint64_t> numbers = {};

    uint64_t curr_num;
    while(input_stream >> curr_num) {
        numbers.emplace_back(curr_num);
    }
    
    return num_stones(numbers, 75);
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
