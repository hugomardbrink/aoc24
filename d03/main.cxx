#include <iostream>
#include <regex>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

int part_1() {
    std::string input = std::string(content);

    auto acc{0};
    std::regex re{"mul\\((\\d+),(\\d+)\\)"};
    std::smatch match;

    while (std::regex_search(input, match, re)) {
        int a = std::stoi(match[1].str());
        int b = std::stoi(match[2].str());
        acc += a * b;

        input = match.suffix().str(); 
    } 

    return acc;
}

int part_2() {
    std::string input = std::string(content);

    auto acc{0};
    std::regex re{"do\\(\\)|don't\\(\\)|mul\\((\\d+),(\\d+)\\)"};
    std::smatch match;
    bool enabled{true};

    while(std::regex_search(input, match, re)) {
        if (match[0].str() == "do()") {
            enabled = true;
        } else if (match[0].str() == "don't()") {
            enabled = false;
        } else if (enabled) {
                int x = std::stoi(match[1].str());
                int y = std::stoi(match[2].str());
                acc += x * y;
        } 
        input = match.suffix().str();
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
