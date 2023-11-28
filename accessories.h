#pragma once
#include <string>
#include <vector>
#include "image.h"
#include <memory>
// #include "player.h"
// #include "world.h"

class Player;

class Accessory {
public:
    std::string name;
    std::string description;
    std::string imagePath;
    float price;
    int levelMin;
    bool bought = false;
    std::unique_ptr<Image> image; // Unique image for each accessory

    Accessory(const std::string& n, const std::string& desc, const std::string& imgPath, float p, int l)
        : name(n), description(desc), imagePath(imgPath), price(p), levelMin(l) {
        image = std::make_unique<Image>(imgPath.c_str());
    }

    virtual ~Accessory() = default;
    virtual void applyEffect(Player& player) = 0; // Making it pure virtual if you want to ensure all accessories have an effect
};

class HeartLocket : public Accessory {
public:
    HeartLocket() : Accessory("Heart Locket", "Boosts health.", "images/heartlocket.png", 10.0f, 1) {}

    void applyEffect(Player& player)  override;
};

class ManaBottle : public Accessory {
public:
    ManaBottle() : Accessory("Bottled Mana", "Boosts Mana", "images/manaBottle.png", 50.0f, 5) {}

    void applyEffect(Player& player)  override;
};

class Magazine : public Accessory {
public:
    Magazine() : Accessory("Magazine", "More Ammo.", "images/magazine.png", 75.0f, 10) {}

    void applyEffect(Player& player)  override;

};

class AccessoryCollection {
public:
    std::vector<std::shared_ptr<Accessory>> accessories;

    AccessoryCollection() {
        // Initialize with all available accessories
        accessories.push_back(std::make_shared<HeartLocket>());
        accessories.push_back(std::make_shared<ManaBottle>());
        accessories.push_back(std::make_shared<Magazine>());
    }

    const std::vector<std::shared_ptr<Accessory>>& getAllAccessories() const {
        return accessories;
    }
};
