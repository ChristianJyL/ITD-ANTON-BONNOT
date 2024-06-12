#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "glad/glad.h"

struct Enemy{
    float x{},y{};
    int hp{};
    float speed{};
    int reward{};
    int currentPathIndex = 0;

    Enemy(float x, float y, int hp, float speed, int reward) : x(x), y(y), hp(hp), speed(speed), reward(reward) {} // Constructor

    void moveX(float dx);
    void moveY(float dy);
    void draw_enemy(float scale , float size, GLuint texture) const;

    bool operator==(const Enemy& other) const {
        return  x == other.x && y == other.y && hp == other.hp && speed == other.speed && reward == other.reward;
    }
};