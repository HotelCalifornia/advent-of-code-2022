#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

enum class Outcome {
    Loss = 0, Tie = 3, Win = 6
};

enum class Move {
    Rock = 1, Paper = 2, Scissors = 3
};

Move get_move(char move) {
    switch (move) {
        case 'A':
        case 'X':
            return Move::Rock;
        case 'B':
        case 'Y':
            return Move::Paper;
        case 'C':
        case 'Z':
            return Move::Scissors;
        default:
            throw std::invalid_argument(std::string{move});
    }
}

std::map<std::pair<Move, Move>, Outcome> results = {
    {{Move::Rock, Move::Rock}, Outcome::Tie},
    {{Move::Rock, Move::Paper}, Outcome::Win},
    {{Move::Rock, Move::Scissors}, Outcome::Loss},
    {{Move::Paper, Move::Rock}, Outcome::Loss},
    {{Move::Paper, Move::Paper}, Outcome::Tie},
    {{Move::Paper, Move::Scissors}, Outcome::Win},
    {{Move::Scissors, Move::Rock}, Outcome::Win},
    {{Move::Scissors, Move::Paper}, Outcome::Loss},
    {{Move::Scissors, Move::Scissors}, Outcome::Tie}
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
       std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
       return -1;
    }

    std::ifstream input(argv[1]);
    if (not input) {
        std::cerr << "Error: could not open file " << argv[1] << std::endl;
    }

    int score = 0;
    for (std::string round; std::getline(input, round); ) {
        auto delim = round.find(' ');
        try {
            Move they = get_move(round.substr(0, delim).at(0));
            Move we = get_move(round.substr(delim + 1).at(0));
            score += static_cast<int>(we) + static_cast<int>(results.find(std::make_pair(they, we))->second);
        } catch(std::invalid_argument& e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
    }

    std::cout << "The elf's strategy guide would net you "
              << score
              << " points in the tourney"
              << std::endl;
}
