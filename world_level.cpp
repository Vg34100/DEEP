//world/level.cpp
#include "world_level.h"

Level::Level(const std::string& name, const std::vector<std::string>& layout, const std::vector<EnemyType>& enemyTypes) 
	: _name(name), _layout(layout), _enemyTypes(enemyTypes) {}

Level Level::SampleLevel() {
	static const std::vector<std::string> layout = {
		// Your level layout here
		"----------NN----------",
		"----------||----------",
		"--xxxxxxxxxxxxxx^^^^--",
		"--x*xxxxxxxxxxxxxx^^--",
		"--xxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--x^^xxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxx*xx|E",
		"W|xxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxx^^xx--",
		"--xxxx--xxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"----------||----------",
		"----------SS----------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
	};
	return Level("SampleLevel", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}

Level Level::PitLand() {
	static const std::vector<std::string> layout = {
		// Your level layout here
		"----------NN----------",
		"----------||----------",
		"--xxxxxxxxxxxxxx^^^^--",
		"--x*xxxxxxxxxxxxxx^^--",
		"--xxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxx*xxxxxxx--",
		"--x^^xxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxx*xxxxxxxx--",
		"W|xxxxxxxxxxxxxxx*xx|E",
		"W|xxxx*xxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxx^^xx--",
		"--xxxx--xxxxxxxxxxxx--",
		"--xxxxxx*xxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxx*xxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxx*xxxx--",
		"--xxxxxxxxxxxxxxxxxx--",
		"----------||----------",
		"----------SS----------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,

	};
	return Level("PitLand", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}
