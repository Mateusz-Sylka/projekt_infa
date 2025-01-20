// GameData.h

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class GameData {
private:
    std::string saveFilePath;
    std::vector<int> levelScores;  // Store scores for each level
    int currentLevel;

public:
    GameData(const std::string& filePath);

    void save(int currentScore, int currentLevel);
    void load(std::vector<int>& scores, int& currentLevel);
    int getLevelScore(int level) const;
    void setLevelScore(int level, int score);
    int getCurrentLevel() const;
    void reset();
};

#endif // GAMEDATA_H
