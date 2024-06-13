#pragma once
#include "Enemy.hpp"
#include <cmath>
#include "glad/glad.h"
#include <unordered_map>

struct Projectile {
    float x, y {};
    float speed {};
    int damage {};
    int targetId {};



    Projectile(float x, float y, float speed, int damage, int targetId) : x(x), y(y), speed(speed), damage(damage), targetId(targetId) {} // Constructor

    void move(float deltaTime ,  const std::vector<Enemy>& enemies);
    bool isArrived( const std::vector<Enemy>& enemies) const;
    bool isTargetAlive(const std::vector<Enemy>& enemies) const;
    int getTypeTower() const;


    void draw_projectile(float scale, float size, std::unordered_map<std::string, GLuint> textures) const;

    bool operator==(const Projectile& other) const {
        return x == other.x && y == other.y && speed == other.speed && damage == other.damage && targetId == other.targetId;
    }


};