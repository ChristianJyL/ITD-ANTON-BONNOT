#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include <iostream>

struct Tower{
    int x,y {};
    int power {};
    int range {};
    int cadence {};
    int cost {};
    double lastAttackTime = 0.0;

    bool isInRange(const Enemy& enemy) const;
    void attack(std::vector<Enemy> &enemies, double currentTime, std::vector<Projectile> &projectiles) ;
};

Tower getTowerLongRange(int x, int y);
Tower getTowerShortRange(int x, int y);
Tower getTowerSlow(int x, int y);
int getTowerRange(int cardSelected);