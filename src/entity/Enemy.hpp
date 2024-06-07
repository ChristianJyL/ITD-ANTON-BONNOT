#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "glm/glm.hpp"

struct Enemy{
    float x,y;
    int pv;
    float speed;
    int reward;
    int currentPathIndex = 0;


    void moveX(float dx){
        x += dx;
    }
    void moveY(float dy){
        y += dy;
    }

    bool operator==(const Enemy& other) const {
        return  x == other.x && y == other.y && pv == other.pv && speed == other.speed && reward == other.reward;
    }
};