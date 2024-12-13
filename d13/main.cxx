#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <ranges>

using std::cout, std::endl;

namespace views = std::ranges::views;

constexpr const char content[] = {
    #embed "input.txt"
};

struct Config {
    std::pair<uint64_t, uint64_t> a_button;
    std::pair<uint64_t, uint64_t> b_button;
    std::pair<uint64_t, uint64_t> prize;
};

uint64_t part_1() {
    std::istringstream input_stream(content);
    std::string line, ignore, val_str;
    std::vector<Config> configs = {};
    while(getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }

        std::istringstream line_stream(line);
        Config config = {};

        line_stream >> ignore >> ignore >> val_str;
        config.a_button.first = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-3));
        line_stream >> val_str;
        config.a_button.second = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-2));

        getline(input_stream, line);
        line_stream = std::istringstream(line);
        line_stream >> ignore >> ignore >> val_str;
        config.b_button.first = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-3));
        line_stream >> val_str;
        config.b_button.second = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-2));

        getline(input_stream, line);
        line_stream = std::istringstream(line);

        line_stream >> ignore >> val_str;
        config.prize.first = std::stoll(val_str.substr(val_str.find('=')+1, val_str.size()-3)); 
        line_stream >> val_str;
        config.prize.second = std::stoll(val_str.substr(val_str.find('=')+1, val_str.size()-2));

        configs.push_back(config);
    }

    constexpr auto a_press_cost = 3;
    constexpr auto b_press_cost = 1;
    uint64_t acc{0};
    
    for(auto& config : configs) {
        int64_t det = config.a_button.first * config.b_button.second - config.a_button.second * config.b_button.first;
        int64_t det_x = config.prize.first * config.b_button.second - config.prize.second * config.b_button.first;
        int64_t det_y = config.a_button.first * config.prize.second - config.prize.first * config.a_button.second;

        if(det == 0) {
            continue;
        } 

        int64_t a_presses = det_x / det;
        int64_t b_presses = det_y / det;

        if(a_presses * config.a_button.first + b_presses * config.b_button.first == config.prize.first 
        && a_presses * config.a_button.second + b_presses * config.b_button.second == config.prize.second) {
            acc += a_presses * a_press_cost + b_presses * b_press_cost;
        }

    } 

    return acc;
}
    

uint64_t part_2() {
    std::istringstream input_stream(content);
    std::string line, ignore, val_str;
    std::vector<Config> configs = {};
    while(getline(input_stream, line)) {
        if(line.empty()) {
            continue;
        }

        std::istringstream line_stream(line);
        Config config = {};

        line_stream >> ignore >> ignore >> val_str;
        config.a_button.first = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-3));
        line_stream >> val_str;
        config.a_button.second = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-2));

        getline(input_stream, line);
        line_stream = std::istringstream(line);
        line_stream >> ignore >> ignore >> val_str;
        config.b_button.first = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-3));
        line_stream >> val_str;
        config.b_button.second = std::stoll(val_str.substr(val_str.find('+')+1, val_str.size()-2));

        getline(input_stream, line);
        line_stream = std::istringstream(line);

        line_stream >> ignore >> val_str;
        config.prize.first = std::stoll(val_str.substr(val_str.find('=')+1, val_str.size()-3)); 
        line_stream >> val_str;
        config.prize.second = std::stoll(val_str.substr(val_str.find('=')+1, val_str.size()-2));

        config.prize.first += 10000000000000;
        config.prize.second += 10000000000000;

        configs.push_back(config);
    }

    constexpr auto a_press_cost = 3;
    constexpr auto b_press_cost = 1;
    uint64_t acc{0};
    
    for(auto& config : configs) {
        int64_t det = config.a_button.first * config.b_button.second - config.a_button.second * config.b_button.first;
        int64_t det_x = config.prize.first * config.b_button.second - config.prize.second * config.b_button.first;
        int64_t det_y = config.a_button.first * config.prize.second - config.prize.first * config.a_button.second;

        if(det == 0) {
            continue;
        } 

        int64_t a_presses = det_x / det;
        int64_t b_presses = det_y / det;

        if(a_presses * config.a_button.first + b_presses * config.b_button.first == config.prize.first 
        && a_presses * config.a_button.second + b_presses * config.b_button.second == config.prize.second) {
            acc += a_presses * a_press_cost + b_presses * b_press_cost;
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
