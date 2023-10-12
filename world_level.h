//world/level.h
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "util_enemytype.h"
#include <random>
#include <algorithm>
#include <unordered_set>

class Level {
private:
	std::string _name;
	std::vector<std::string> _layout;
	std::vector<EnemyType> _enemyTypes;

public:
	Level(const std::string& name, const std::vector<std::string>& layout, const std::vector<EnemyType>& enemyTypes);

	const std::string& name() const { return _name; }
	const std::vector<std::string>& layout() const { return _layout; }
	const std::vector<EnemyType>& enemyTypes() const { return _enemyTypes; }
	
	
	void printLevel() const {
		for (const auto& row : _layout) {
			printf("%s\n", row.c_str());
		}
	}
	
	int getRows() const {
		return _layout.size();
	}

	int getColumns() const {
		if (_layout.empty()) return 0;
		return _layout[0].size();
	}

	static Level SampleLevel();  // A method to generate a sample level
	// Add more static methods for other levels
	static Level PitLand();

	static std::vector<Level> getAllAvailableLevels() {
		return {
			SampleLevel(),
			PitLand(),
			// Add any other levels you create in the future here.
		};
	}

	


};


class LevelManager {
private:
	std::unordered_set<std::string> usedLevels;  // Assuming level identifiers are strings, you can use another type if needed.
public:

	Level getRandomLevel() {
		std::vector<Level> allLevels = Level::getAllAvailableLevels();
		
		std::shuffle(allLevels.begin(), allLevels.end(), std::default_random_engine(std::random_device()()));

		for (const Level& level : allLevels) {
			if (usedLevels.find(level.name()) == usedLevels.end()) {  
				usedLevels.insert(level.name());
				return level;
			}
		}

		// If all levels are exhausted
		throw std::runtime_error("All levels have been used!");
	}


};


