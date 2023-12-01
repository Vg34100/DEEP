//world/level.cpp
#include "world_level.h"

Level::Level(const std::string& name, const std::vector<std::string>& layout, const std::vector<EnemyType>& enemyTypes) 
	: _name(name), _layout(layout), _enemyTypes(enemyTypes) {}

Level Level::SampleLevel() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--xxxxxxxxxxxxxxxxxx^^^^--",
		"--xxxxxxxxxxxxxxxxxxxx^^--",
		"--xxxxxxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxx*xxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--x^^xxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxx^^xx--",
		"--xxxx--xxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxx*xxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("SampleLevel", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}

Level Level::PitLand() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--x^^^^^xxxxxxxxxxxx^^^^--",
		"--xx^^^xxxxxxxxxxxxxxx^^--",
		"--xxxxxxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxx*xxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxx*xxxxxxxxxxxxxxx--",
		"--x^^xxxxxxxxxxxxxxxx*xx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxx^^xx--",
		"--xxxx--xxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxx*xxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx*xxx--",
		"--xxxxx^^xxxxxxxxxxxxxxx--",
		"--xxxx^^^^xxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxx^^^^x--",
		"--xxxxxxxxxxxxxxx^^^^^^^--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("PitLand", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}

Level Level::SampleLevel2() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxx^^xxx--",
		"--xxx^^^^^xxxxxxxxx^^^xx--",
		"--xxxx^^^^xxxxxxxxxxxxxx--",
		"--xxx^^^xxxxxxxxxxxx*xxx--",
		"--xxxx-----xxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--x^^xxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxx^^xx--",
		"--xxxx--xxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxx*x--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xx*xxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxx-------xx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxx^^^^xxx--",
		"--xxxxxxxxxxxxxxxx^^^xxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("SampleLevel2", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}

Level Level::EmptyLevel() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
	};
	return Level("SampleLevel2", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}


Level Level::SampleLevel3() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxx*xxxxxxxxxx-xxxxxx--",
		"--xxxxxxxxxxxxxxx-xxxxxx--",
		"--xxxxxxxxxxxxxxx-xx*xxx--",
		"--xxxxxxxxxxxxxxx-xxxxxx--",
		"--xxx*xxxxxxxxxxxxxxxxxx--",
		"--xxxxx-xxxxxxxxxxxxxxxx--",
		"--xxxxx-xxxxxxxxxxxxxxxx--",
		"--xxxxx-xxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxx------xxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xx-------xxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("SampleLevel3", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}


Level Level::SampleLevel4() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--^^^^^^^xxxxxxxxxxxxx^^--",
		"--^^^^xxxxxxxxxxxxxxxx^^--",
		"--^xxxxxxxxxxxxxxxxxxx^^--",
		"--^xxxxxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxx*xxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxx*xxx--",
		"--xxxx*xxxx-----xxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxx*xxxxxxxxxxxx*xxx--",
		"--xxxxxxxxxx----xxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxx*xxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxx^--",
		"--xxxxxxxxxxxxxxxxxxxx^^--",
		"--^^^xxxxxxxxxxxxxxxxx^^--",
		"--^^^^^xxxxxxxxxxxxxx^^^--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("SampleLevel4", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}


Level Level::SampleLevel5() 
{
	static const std::vector<std::string> layout = {
		// Your level layout here
		"------------NN------------",
		"------------||------------",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxx-xxxxx--",
		"--xxxx*xxxxxxxxxxx-xxxxx--",
		"--xxxxxxxxxxxxxxxx-xxxxx--",
		"--xxx---xxxxxxxxxx-xxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxx*xxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"W|xxxxxxxxxxxxxxxxxxxxxx|E",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxx-xxxxxxxxx*xxxxx--",
		"--xxxxxx-xxxxxxxxxxxxxxx--",
		"--xxxxxx-xxxxxxxxxxxxxxx--",
		"--xxxxxx-xxxxxxxxx---xxx--",
		"--xxxxxx-xxxxxxxxxxxxxxx--",
		"--xxxxxx-xxxxxxxxxxxx*xx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"--xxxxxxxxxxxxxxxxxxxxxx--",
		"------------||------------",
		"------------SS------------"
	};
	static const std::vector<EnemyType> enemyTypes = {
		// Your enemy types here
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
		EnemyType::SLIME,
	};
	return Level("SampleLevel5", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}