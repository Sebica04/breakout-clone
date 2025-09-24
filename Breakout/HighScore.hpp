#pragma once
#include <vector>
#include <string>


struct ScoreEntry {
    int score;
};

namespace HighScore
{
    void loadScores(std::vector<ScoreEntry>& scores);

    void saveScores(const std::vector<ScoreEntry>& scores);

    bool updateAndCheck(int newScore, std::vector<ScoreEntry>& scores);

}