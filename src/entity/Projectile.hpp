#pragma once
#include "Enemy.hpp"
#include <cmath>
#include "glad/glad.h"

struct Projectile {
    float x, y {};
    float speed {};
    Enemy* target {};

    Projectile(float x, float y, float speed, Enemy* target) : x(x), y(y), speed(speed), target(target) {} // Constructor

    void move(float deltaTime);
    bool isArrived() const;
    void draw_projectile(float scale, float size) const;

    bool operator==(const Projectile& other) const {
        return x == other.x && y == other.y && speed == other.speed && target == other.target;
    }


};