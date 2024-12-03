#include <iostream>
#include <regex>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

void part_1() {
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

    cout << acc << endl;
}

void part_2() {
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
    
    cout << acc << endl;
} 

int main() {
    part_1();
    part_2();
    return 0;
}
