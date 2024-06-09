#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include <iostream>

struct Tower{
    int x,y;
    int power;
    int range;
    int cadence;
    float lastAttackTime = 0;

    Tower(int x, int y, int power, int range, int cadence) : x(x), y(y), power(power), range(range), cadence(cadence) {} // Constructor

    bool isInRange(const Enemy& enemy) const;
    void attack(std::vector<Enemy> &enemies, float currentTime, std::vector<Projectile> &projectiles) ;

};

