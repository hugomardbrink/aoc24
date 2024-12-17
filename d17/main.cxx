#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <omp.h>

using std::cout, std::endl;

constexpr const char content[] = {
    #embed "input.txt"
};

enum class Opcode : int {
    ADV = 0,
    BXL,
    BST,
    JNZ,
    BXC,
    OUT,
    BDV,
    CDV
};

struct Registers {
    uint64_t A;
    uint64_t B;
    uint64_t C;
    uint64_t instruction_pointer;
    std::string rstdout = "";

    bool operator==(const Registers& other) const {
        return A == other.A && B == other.B && C == other.C && instruction_pointer == other.instruction_pointer && rstdout == other.rstdout;
    }
};

namespace std {
    template <>
    struct hash<Registers> {
        std::size_t operator()(const Registers& regs) const {
            return std::hash<uint64_t>{}(regs.A) ^ std::hash<uint64_t>{}(regs.B) ^ std::hash<uint64_t>{}(regs.C) ^ std::hash<std::string>{}(regs.rstdout);
        }
    };
}

constexpr uint64_t resolve_combo_operand(int operand, Registers& regs) {
    switch(operand) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return regs.A;
        case 5: return regs.B;
        case 6: return regs.C;
        default: exit(-1);
    };
}

std::unordered_map<Opcode, std::function<void(Registers& regs, int64_t operand)>> opcodes = {
    {Opcode::ADV, [](Registers& regs, int64_t operand) { 
        regs.A /= std::pow(2, resolve_combo_operand(operand, regs)); 
        regs.instruction_pointer += 2; 
    }},
    {Opcode::BXL, [](Registers& regs, int64_t operand) { 
        regs.B ^= operand;
        regs.instruction_pointer += 2; 
    }},
    {Opcode::BST, [](Registers& regs, int64_t operand) { 
        regs.B = resolve_combo_operand(operand, regs) % 8; 
        regs.instruction_pointer += 2; 
    }},
    {Opcode::JNZ, [](Registers& regs, int64_t operand) { 
        if(regs.A != 0) {
            if(regs.instruction_pointer == operand) {
                regs.instruction_pointer += 2;
            } else {
                regs.instruction_pointer = operand;
            }
        } 
    }},
    {Opcode::BXC, [](Registers& regs, int64_t _) { 
        regs.B ^= regs.C;
        regs.instruction_pointer += 2; 
    }},
    {Opcode::OUT, [](Registers& regs, int64_t operand) { 
        auto val_str = std::to_string(resolve_combo_operand(operand, regs) % 8);
        std::string to_print = "";
        for(int i = 0; i < val_str.size(); i++) {
            to_print += val_str[i];
            to_print += ',';
        }
        regs.rstdout += to_print;
        regs.instruction_pointer += 2; 
    }},
    {Opcode::BDV, [](Registers& regs, int64_t operand) { 
        regs.B = regs.A / std::pow(2, resolve_combo_operand(operand, regs)); 
        regs.instruction_pointer += 2; 
    }},
    {Opcode::CDV, [](Registers& regs, int64_t operand) { 
        regs.C = regs.A / std::pow(2, resolve_combo_operand(operand, regs)); 
        regs.instruction_pointer += 2; 
    }}
}; 

std::string part_1() {
    std::istringstream input_stream(content);
    std::string line;

    Registers regs = {0, 0, 0, 0, ""};
    std::getline(input_stream, line);
    regs.A = std::stoll(line.substr(line.find(":") + 1));
    
    std::getline(input_stream, line);
    regs.B = std::stoll(line.substr(line.find(":") + 1));

    std::getline(input_stream, line);
    regs.C = std::stoll(line.substr(line.find(":") + 1));

    std::vector<int> program;
    getline(input_stream, line);
    getline(input_stream, line, ' ');
    while(getline(input_stream, line, ',')) {
        if(!line.empty()) program.push_back(std::stoi(line));
    }

    auto prev_regs = regs;
    while(regs.instruction_pointer < program.size()) {
        auto opcode = static_cast<Opcode>(program[regs.instruction_pointer]);
        auto operand = program[regs.instruction_pointer + 1];
        opcodes[opcode](regs, operand);

        if(prev_regs == regs) {
            regs.rstdout.pop_back();
            break;
        }
        prev_regs = regs;
    }

    return regs.rstdout;
}

uint64_t part_2() {
    std::istringstream input_stream(content);
    std::string line;

    Registers target_regs = {0, 0, 0, 0, ""};
    std::getline(input_stream, line);
    target_regs.A = std::stoll(line.substr(line.find(":") + 1));
    
    std::getline(input_stream, line);
    target_regs.B = std::stoll(line.substr(line.find(":") + 1));

    std::getline(input_stream, line);
    target_regs.C = std::stoll(line.substr(line.find(":") + 1));

    std::vector<int> program;
    getline(input_stream, line);
    getline(input_stream, line, ' ');
    while(getline(input_stream, line, ',')) {
        if(!line.empty()) program.push_back(std::stoi(line));
    }
    for(auto i : program) {
        target_regs.rstdout += std::to_string(i) + ",";
    }
    target_regs.rstdout.pop_back();

    uint64_t a_init = std::pow(8, 15);
    while(a_init < std::pow(8, 16)){
        Registers regs = {a_init, 0, 0, 0, ""};
        auto prev_regs = regs;
        while(regs.instruction_pointer < program.size()) {
            auto opcode = static_cast<Opcode>(program[regs.instruction_pointer]);
            auto operand = program[regs.instruction_pointer + 1];
            opcodes[opcode](regs, operand);
            if(prev_regs == regs) {
                regs.rstdout.pop_back();
                break;
            }
            prev_regs = regs;
        }

        if(regs.rstdout == target_regs.rstdout) {
            break;
        }
        
        for(int i = 15; i >= 0; i--) {
            if(regs.rstdout[i*2] != target_regs.rstdout[i*2]) {
                a_init += std::pow(8, i); 
                break;
            }
        }
    }

    return a_init;
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

