//world/level.cpp
#include "level.h"

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
        EnemyType::ORC,
        EnemyType::ORC
    };
    return Level("SampleLevel", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}

Level Level::PitLand() {
    static const std::vector<std::string> layout = {
        // Your level layout here
        "-------------NN-------------",
        "-------------||-------------",
        "--xxxxxxxxxxxxxxxxxxxx^^^^--",
        "--x*xxxxxxxxxxxxxxxxxxxx^^--",
        "--xxxxxxxxxxxxxxxxx--xxxx^--",
        "--xxxxxxxxxxxxxxx----xxxxx--",
        "--xxx^^^xxxxxxxxxxxxxxxxxx--",
        "--x^^^^^^xxxxxxxxxxxxx^^xx--",
        "--xx^^^xxxxxxxxxxxxxx^^^xx--",
        "--xxxxxxxxxxxxxxxxxxxxxxxx--",
        "--xxxxxxxxxxxxxxxxxxxxx*xx--",
        "--xxxxxxxxxxxxxxxxxxxxxxxx--",
        "--xxxxx---xxxxxxxxxxxx^^xx--",
        "W|xxxx----xxxxxxxxxxxxxxxx|E",
        "W|xxxxxxxxxxxxxxxxxxxxxxxx|E",
        "--xxxxxxxxxxxxxxxxxxxxxxxx--",
        "--xxxxxxxxxxxxxxxxxx^^xxxx--",
        "--xxxxxxxxxxxxxxxx^^^^^xxx--",
        "--xxxxxxxxxxxxxxxx^^^^^xxx--",
        "--^xxxx*xxxxxxxxxxxx^^^xxx--",
        "--^xxxxxxxxxxxxxxxxxxxxxxx--",
        "--^xxxxxxxxxxxxxxxxxxxxxxx--",
        "--^xxxxxxxxxxxxxxxxxxxxx*x--",
        "--^^^xxxxxxxxxxxxx---xxxxx--",
        "--^^^^xxxxxxxxxx------xxxx--",
        "--^^^^^xxxxxxxx---------xx--",
        "-------------||-------------",
        "-------------SS-------------"
    };
    static const std::vector<EnemyType> enemyTypes = {
        // Your enemy types here
        EnemyType::ORC,
        EnemyType::ORC,
        EnemyType::ORC,
        EnemyType::ORC

    };
    return Level("PitLand", layout, enemyTypes);  // "SampleLevel" is the hard-coded name for this level.
}
