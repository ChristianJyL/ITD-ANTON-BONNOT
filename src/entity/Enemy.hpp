#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "glad/glad.h"

struct Enemy{
    int spawnIndex{};
    float x{},y{};
    int hp{};
    float speed{};
    int reward{};
    int currentPathIndex = 0;
     int uniqueId  = generateUniqueId();

    Enemy(int spawnIndex, float x, float y, int hp, float speed, int reward) : spawnIndex(spawnIndex), x(x), y(y), hp(hp), speed(speed), reward(reward) {} // Constructor

    void moveX(float dx);
    void moveY(float dy);
    void draw_enemy(float scale , float size, GLuint texture) const;

    bool operator==(const Enemy& other) const {
        return uniqueId == other.uniqueId;
    }

    static int generateUniqueId(){
        static int id = 0;
        return id++;
    }
};