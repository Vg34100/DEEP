//world/level.h
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "enemytype.h"
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
    enum DoorPosition {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    struct DoorInfo {
        DoorPosition position;
        int index;

        void print() const {
            switch (position) {
                case TOP:
                    std::cout << "Top door found at position: " << index << std::endl;
                    break;
                case BOTTOM:
                    std::cout << "Bottom door found at position: " << index << std::endl;
                    break;
                case LEFT:
                    std::cout << "Left door found at row: " << index << std::endl;
                    break;
                case RIGHT:
                    std::cout << "Right door found at row: " << index << std::endl;
                    break;
            }
        }
    };

    std::vector<DoorInfo> findDoors(const Level& level) {
        const std::vector<std::string>& layout = level.layout();
        std::vector<DoorInfo> doors;

        // Check top row
        size_t topDoor = layout.front().find("||");
        if (topDoor != std::string::npos) {
            doors.push_back({TOP, static_cast<int>(topDoor)});
        }

        // Check bottom row
        size_t bottomDoor = layout.back().find("||");
        if (bottomDoor != std::string::npos) {
            doors.push_back({BOTTOM, static_cast<int>(bottomDoor)});
        }

        // Check first and last columns for vertical doors
        for (size_t i = 0; i < layout.size(); i++) {
            if (i < layout.size() - 1 && layout[i][0] == '|' && layout[i + 1][0] == '|') {
                doors.push_back({LEFT, static_cast<int>(i)});
            }

            if (i < layout.size() - 1 && layout[i].back() == '|' && layout[i + 1].back() == '|') {
                doors.push_back({RIGHT, static_cast<int>(i)});
            }
        }

        return doors;
    }


    std::vector<LevelManager::DoorInfo> getPossiblePairings(const Level& l1, const Level& l2) {
        std::vector<LevelManager::DoorInfo> doors1 = findDoors(l1);
        std::vector<LevelManager::DoorInfo> doors2 = findDoors(l2);

        std::vector<LevelManager::DoorInfo> pairings;

        for (auto door1 : doors1) {
            for (auto door2 : doors2) {
                if ((door1.position == LevelManager::TOP && door2.position == LevelManager::BOTTOM) ||
                    (door1.position == LevelManager::BOTTOM && door2.position == LevelManager::TOP) ||
                    (door1.position == LevelManager::LEFT && door2.position == LevelManager::RIGHT) ||
                    (door1.position == LevelManager::RIGHT && door2.position == LevelManager::LEFT)) {
                    
                    pairings.push_back(door1);
                    pairings.push_back(door2);

                    std::cout << "Pairing found: " << std::endl;
                    door1.print();
                    door2.print();
                    std::cout << "--------------------" << std::endl;
                }
            }
        }
        return pairings;
    }

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

class LevelCombiner {
    LevelManager manager;

public:

};


