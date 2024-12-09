#include <iostream>
#include <chrono>
#include <vector>
#include <optional>
#include <ranges>
#include <algorithm>


using std::cout, std::endl;
namespace views = std::ranges::views;

constexpr const char content[] = {
    #embed "input.txt"
};

void fill_free_disk(std::vector<std::optional<int>>& disk) {
    for(auto i{0}; i < disk.size(); i++) {
        if(!disk[i].has_value()) {
            for(auto j{disk.size() - 1}; j > i; j--) {
                if(disk[j].has_value() && j > i) {
                    disk[i] = disk[j];
                    disk[j] = std::nullopt;
                    break;
                }
            }
        }
    }
}

long part_1() {
    std::vector<std::optional<int>> disk = {};
    
    int id_counter{1};
    for(int i = 0; i < sizeof(content); i++) {
        std::optional<int> id = i % 2 == 0 ? std::make_optional(i/2) : std::nullopt;
       for(int j = 0; j < content[i] - '0'; j++) {
           disk.push_back({id});
       }
    }
    
    fill_free_disk(disk);
    
    long checksum{0};
    for(int i{0}; i < disk.size(); i++) {
        if(disk[i].has_value()) { 
            checksum += i * disk[i].value();
        }
    }

    return checksum;
}

struct Partition {
    std::optional<int> id;
    int start_index;
    int size;

    bool operator==(const Partition& other) const {
        return id == other.id && start_index == other.start_index;
    }
};
long part_2() {
    std::vector<Partition> partitions = {};

    for(int i = 0; i < sizeof(content)-1; i++) {
        std::optional<int> id = i % 2 == 0 ? std::make_optional(i/2) : std::nullopt;
        partitions.push_back({id, i, content[i] - '0'});
    }
    
   for(auto& free_space : partitions) {
        if(!free_space.id.has_value()) {
            for(auto& file : partitions | views::reverse) {
                if(file.id.has_value() && file.size <= free_space.size && file.start_index > free_space.start_index) {
                    int space_left = free_space.size - file.size;
                    Partition move_file = file; 
                    move_file.start_index = free_space.start_index;
                    file.id = std::nullopt;

                    if(space_left == 0) {
                        partitions.erase(std::find(partitions.begin(), partitions.end(), free_space));
                    } else {
                        free_space.size = space_left;
                        free_space.start_index += file.size;
                    }
                    
                    partitions.insert(std::find(partitions.begin(), partitions.end(), free_space), move_file);
                    break;
                }
            } 
        }
    } 
    
    int counter{0};
    long checksum{0};
    for(auto& partition : partitions) {
       for(auto _ : views::iota(0, partition.size)) {
           if(partition.id.has_value()) {
                checksum += counter * partition.id.value();
           }
           counter++;
       }
    }
    return checksum;
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
