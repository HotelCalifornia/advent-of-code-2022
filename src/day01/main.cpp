#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
        return -1;
    }

	std::ifstream input(argv[1]);
    if (not input) {
        std::cerr << "Error: could not open file " << argv[1] << std::endl;
    }

    std::vector<int> rations;
    int ration_accumulator = 0;
    
    // each elf's ration list is separated from the others by an extra blank
    // line, so accumulate values until we hit one and then add the accumulated
    // value to our list of ration totals
    for (std::string line; std::getline(input, line); ) {
        if (line.length() == 0) {
            rations.push_back(ration_accumulator);
            ration_accumulator = 0;
        } else {
            ration_accumulator += std::stoi(line);
        }
    }

    std::sort(rations.begin(), rations.end(), std::greater<int>());

    std::cout << "The elf with the fattest pack is carrying "
              << *rations.begin()
              << " calories"
              << std::endl;

    std::cout << "The top three elves together are carrying "
              << std::accumulate(rations.begin(), rations.begin() + 3, 0)
              << " calories, total"
              << std::endl;
}
