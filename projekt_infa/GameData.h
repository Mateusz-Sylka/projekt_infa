#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

class GameData {
private:
    std::string filename;

public:
    GameData(const std::string& file) : filename(file) {}

    // Save scores for all levels and the current level to file
    void save(const std::vector<int>& scores, int level) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << level << std::endl; // Save the current level
            for (const int& score : scores) {
                file << score << " "; // Save all level scores in a single line
            }
            file << std::endl;
            file.close();
            std::cout << "Game data saved to " << filename << std::endl;
        }
        else {
            std::cerr << "Error: Unable to save game data to " << filename << std::endl;
        }
    }

    // Load scores for all levels and the current level from file
    bool load(std::vector<int>& scores, int& level) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> level; // Load the current level
            scores.clear(); // Clear the scores vector
            std::string line;
            std::getline(file, line); // Move to the next line for scores
            std::getline(file, line);
            std::istringstream iss(line);
            int score;
            while (iss >> score) {
                scores.push_back(score);
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
