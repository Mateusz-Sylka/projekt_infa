// GameData.cpp

#include "GameData.h"

GameData::GameData(const std::string& filePath) : saveFilePath(filePath) {
    std::ifstream file(saveFilePath);
    if (file.is_open()) {
        file >> currentLevel; // Read current level
        levelScores.clear();
        int score;
        while (file >> score) {
            levelScores.push_back(score);
        }
        file.close();
        std::cout << "Loaded GameData: Level " << currentLevel << ", Scores: ";
        for (int s : levelScores) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    else {
        currentLevel = 0; // Default values if no save file is present
        levelScores = std::vector<int>(4, 0);
        std::cout << "No save file found. Initialized with default values." << std::endl;
    }
}


void GameData::save(int currentScore, int currentLevel) {
    std::ofstream file(saveFilePath); // Open the file for writing
    if (file.is_open()) {
        file << currentLevel << std::endl; // Save the current level first
        for (size_t i = 0; i < levelScores.size(); ++i) {
            file << levelScores[i];
            if (i < levelScores.size() - 1) {
                file << " "; // Add a space between scores (but not at the end)
            }
        }
        file << std::endl; // End the scores line
        file.close();
    }
}



void GameData::load(std::vector<int>& scores, int& currentLevel) {
    std::ifstream saveFile(saveFilePath);
    if (saveFile.is_open()) {
        saveFile >> currentLevel; // Load current level first
        scores.clear();
        int score;
        while (saveFile >> score) {
            scores.push_back(score);
        }
        saveFile.close();
    }
    else {
        std::cout << "Failed to open save file.\n";
    }
}



int GameData::getLevelScore(int level) const {
    return levelScores[level];
}

void GameData::setLevelScore(int level, int score) {
    if (level < levelScores.size()) {
        levelScores[level] = score;
    }
}

int GameData::getCurrentLevel() const {
    return currentLevel;
}

void GameData::reset() {
    levelScores = std::vector<int>(4, 0);
    currentLevel = 0;
}
