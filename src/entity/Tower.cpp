#include "Tower.hpp"

bool Tower::isInRange(const Enemy& enemy) const {
    int distance = std::max(std::abs(x-enemy.x) , std::abs(y - enemy.y));
    return distance <= range;
}

void Tower::attack(std::vector<Enemy> &enemies , float currentTime , std::vector<Projectile> &projectiles)  {
    for (Enemy& enemy : enemies) {
        if (isInRange(enemy) && currentTime - lastAttackTime >= cadence) {
            enemy.hp -= power;
            lastAttackTime = currentTime;
            projectiles.push_back(Projectile(x, y, 10, &enemy));
        }
    }
}

Tower getTowerLongRange(int x, int y) {
    return Tower(x, y, 3, 5, 2);
}

Tower getTowerShortRange(int x, int y) {
    return Tower(x, y, 10, 2, 1);
}

Tower getTowerSlow(int x, int y) {
    return Tower(x, y, 6, 1, 3);
}

int getTowerRange(int cardSelected) {
    switch (cardSelected) {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 5;
        default:
            return 0;
    }
}