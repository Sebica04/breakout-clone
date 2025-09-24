#include "HighScore.hpp"
#include <fstream>
#include <algorithm>

const std::string FILENAME = "assets/highscores.txt";
const int MAX_SCORES = 5;

void HighScore::loadScores(std::vector<ScoreEntry>& scores) {
	scores.clear();
	std::ifstream file(FILENAME);
	int score;
	while (file >> score) {
		scores.push_back({ score });
	}
}

void HighScore::saveScores(const std::vector<ScoreEntry>& scores) {
	std::ofstream file(FILENAME);
	for (const auto& entry : scores) {
		file << entry.score << std::endl;
	}
}


bool HighScore::updateAndCheck(int newScore, std::vector<ScoreEntry>& scores) {
	// Check if the score is high enough to even be on the list.
	// If the list isn't full, or if the new score is better than the worst score.
	bool isNewHighScore = false;
	if (scores.size() < MAX_SCORES || newScore > scores.back().score) {
		isNewHighScore = true;
	}
	else {
		return false; // Not a high score, do nothing.
	}

	scores.push_back({ newScore });

	std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
		return a.score > b.score;
		});

	if (scores.size() > MAX_SCORES) {
		scores.resize(MAX_SCORES);
	}

	return isNewHighScore;
}