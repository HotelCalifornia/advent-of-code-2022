#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

enum class Outcome {
    Loss = 0, Tie = 3, Win = 6
};

namespace outcome {
    std::string to_string(const Outcome& o) {
        switch (o) {
            case Outcome::Loss: return "Loss";
            case Outcome::Tie: return "Tie";
            case Outcome::Win: return "Win";
        }
    }

    Outcome make_outcome(char outcome) {
        switch (outcome) {
            case 'X': return Outcome::Loss;
            case 'Y': return Outcome::Tie;
            case 'Z': return Outcome::Win;
            default: throw std::invalid_argument(std::string{outcome});
        }
    }
}

enum class Move {
    Rock = 1, Paper = 2, Scissors = 3
};

namespace move {
    std::string to_string(const Move& m) {
        switch (m) {
            case Move::Rock: return "Rock";
            case Move::Paper: return "Paper";
            case Move::Scissors: return "Scissors";
        }
    }

    Move make_move(char move) {
        switch (move) {
            case 'A': return Move::Rock;
            case 'B': return Move::Paper;
            case 'C': return Move::Scissors;
            default: throw std::invalid_argument(std::string{move});
        }
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

Move get_move(Move they, Outcome desired) {
    auto result = std::find_if(results.begin(), results.end(), [&] (const auto& e) {
        return e.second == desired && e.first.first == they;
    });

    if (result == results.end()) {
        throw std::range_error(
                std::string{"no move found for combination they<"} +
                move::to_string(they) + std::string{"> and outcome<"} +
                outcome::to_string(desired) +
                std::string{">"}
        );
    }

    return result->first.second;
}


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
            Move they = move::make_move(round.substr(0, delim).at(0));
            Move we = get_move(they, outcome::make_outcome(round.substr(delim + 1).at(0)));
            score += static_cast<int>(we) + static_cast<int>(results.find(std::make_pair(they, we))->second);
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
    }

    std::cout << "The elf's strategy guide would net you "
              << score
              << " points in the tourney"
              << std::endl;
}
