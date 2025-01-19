#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class GameData {
private:
    std::string filename;

public:
    GameData(const std::string& file) : filename(file) {}

    // Save score, level, and coin states to file
    void save(int score, int level, const std::vector<bool>& coinsCollected) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << score << std::endl;
            file << level << std::endl;

            // Save coin states
            for (bool state : coinsCollected) {
                file << state << " ";  // Store the state of each coin (true/false)
            }
            file << std::endl;
            file.close();
            std::cout << "Game data saved to " << filename << std::endl;
        }
        else {
            std::cerr << "Error: Unable to save game data to " << filename << std::endl;
        }
    }

    // Load score, level, and coin states from file
    bool load(int& score, int& level, std::vector<bool>& coinsCollected) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> score;
            file >> level;

            // Load coin states
            coinsCollected.clear();
            bool state;
            while (file >> state) {
                coinsCollected.push_back(state);
            }

            file.close();
            std::cout << "Game data loaded from " << filename << std::endl;
            return true;
        }
        else {
            std::cerr << "Error: No saved game found in " << filename << std::endl;
            return false;
        }
    }
};

#endif // GAMEDATA_H
