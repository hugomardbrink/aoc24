#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <functional>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

std::vector<std::vector<uint64_t>> generate_combinations(int n, int k) {
    std::vector<std::vector<uint64_t>> result = {};
    std::vector<uint64_t> current(n, 0);

    while (true) {
        result.push_back(current);

        int pos = n - 1;
        while (pos >= 0 && current[pos] == k - 1) {
            pos--;
        }

        if (pos < 0) break; 

        current[pos]++;
        for (uint64_t i = pos + 1; i < n; i++) {
            current[i] = 0;
        }
    }

    return result;
}

uint64_t part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> defined_ops = {
        [](uint64_t x, uint64_t y) { return x + y; },  
        [](uint64_t x, uint64_t y) { return x * y; },    
    };

    uint64_t acc{0};
    while (std::getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::istringstream line_stream(line);
        std::vector<uint64_t> values = {};
        
        std::string test_string;
        line_stream >> test_string;
        uint64_t val;
        while (line_stream >> val) {
            values.push_back(val);
        }

        uint64_t test = std::stoull(test_string);
        std::vector<std::vector<uint64_t>> op_combinations = generate_combinations(values.size() - 1, defined_ops.size());
        
        for(auto& op_comb : op_combinations) {
            uint64_t result = values[0];
            for(uint64_t i = 1; i < values.size(); i++) {
                result = defined_ops[op_comb[i-1]](result, values[i]);
            }
            if(result == test) {
                acc += test;
                break;
            }
        }
    }
    
    return acc;
}

uint64_t part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> defined_ops = {
        [](uint64_t x, uint64_t y) { return x + y; },  
        [](uint64_t x, uint64_t y) { return x * y; },    
        [](uint64_t x, uint64_t y) { return std::stoull(std::to_string(x) + std::to_string(y)); },
    };

    uint64_t acc{0};
    while (std::getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }
        std::istringstream line_stream(line);
        std::vector<uint64_t> values = {};
        
        std::string test_string;
        line_stream >> test_string;
        uint64_t val;
        while (line_stream >> val) {
            values.push_back(val);
        }

        uint64_t test = std::stoull(test_string);
        std::vector<std::vector<uint64_t>> op_combinations = generate_combinations(values.size() - 1, defined_ops.size());
        
        for(auto& op_comb : op_combinations) {
            uint64_t result = values[0];
            for(uint64_t i = 1; i < values.size(); i++) {
                result = defined_ops[op_comb[i-1]](result, values[i]);
                if(result > test) break; 
            }
            if(result == test) {
                acc += test;
                break;
            }
        }
    }
    
    return acc;
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
