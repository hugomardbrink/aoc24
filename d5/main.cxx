#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm> 
#include <chrono>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

int part_1() {
    std::istringstream input_stream(content);
    std::string line;
    std::unordered_map<int, std::vector<int>> pages = {};
    std::vector<std::vector<int>> updates = {}; 


    while (std::getline(input_stream, line)) {
       if(line.empty()) {
           break;
       }
       auto fst_page = std::stoi(line.substr(0, 2));
       auto snd_page = std::stoi(line.substr(3, 4));
       if(pages.find(fst_page) == pages.end()) {
           pages[fst_page] = {};
       }
       pages[snd_page].emplace_back(fst_page);
    }

    while (std::getline(input_stream, line)) {
        size_t start{0}, end{0};
        std::vector<int> update = {};
        while(true) {
            end = line.find(",", start);
            if(end == std::string::npos) {
                update.push_back(std::stoi(line.substr(start, end)));
                break;
            }
            update.push_back(std::stoi(line.substr(start, end)));
            start = end + 1;
        }
        updates.push_back(update);
    }

    int acc{0};
    for(const auto& update : updates) {
        bool invalid_update = false;
        for(auto num_it = update.begin(); num_it != update.end(); num_it++) {
            auto num = *num_it;
            invalid_update = std::find_if(pages[num].begin(), pages[num].end(), [&update, num_it](int precede_num) {
                    auto precede_it = std::find(update.begin(), update.end(), precede_num);
                    if(precede_it != update.end() && precede_it > num_it) {
                        return true;
                    } else {
                        return false;
                    }
            }) != pages[num].end();  
            

            if(invalid_update) {
                break;
            } 
        }

        int middle_size = update[update.size() / 2];
        if(!invalid_update) {
            acc += middle_size;
        }
    }
    return acc;
}

int part_2() {
    std::istringstream input_stream(content);
    std::string line;
    std::unordered_map<int, std::vector<int>> pages = {};
    std::vector<std::vector<int>> updates = {}; 

    while (std::getline(input_stream, line)) {
       if(line.empty()) {
           break;
       }
       auto fst_page = std::stoi(line.substr(0, 2));
       auto snd_page = std::stoi(line.substr(3, 4));
       if(pages.find(fst_page) == pages.end()) {
           pages[fst_page] = {};
       }
       pages[snd_page].emplace_back(fst_page);
    }

    while (std::getline(input_stream, line)) {
        size_t start{0}, end{0};
        std::vector<int> update = {};
        while(true) {
            end = line.find(",", start);
            if(end == std::string::npos) {
                update.push_back(std::stoi(line.substr(start, end)));
                break;
            }
            update.push_back(std::stoi(line.substr(start, end)));
            start = end + 1;
        }
        updates.push_back(update);
    }

    int acc{0};
    for(std::vector<int>& update : updates) {
        bool invalid_update = false;

        std::sort(update.begin(), update.end(), [&invalid_update, update, &pages](int a, int b) {
            bool contains = std::find(pages[b].begin(), pages[b].end(), a) != pages[b].end();
            if(contains) {
                auto start_it = std::find(update.begin(), update.end(), b);
                for(auto cmp_num = start_it + 1; cmp_num != update.end(); cmp_num++) {
                    if(*cmp_num == a) {
                        invalid_update = true;
                        return true;
                    }
                }
            }
            return false;
        });

        int middle_size = update[update.size() / 2];
        if(invalid_update) {
            acc += middle_size;
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
