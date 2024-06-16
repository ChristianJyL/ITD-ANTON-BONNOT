#include "Tower.hpp"

bool Tower::isInRange(const Enemy& enemy) const {
    int distance = std::max(std::abs(x-static_cast<int>(enemy.x)) , std::abs(y - static_cast<int>(enemy.y)));
    return distance <= range;
}

void Tower::attack(std::vector<Enemy> &enemies , double currentTime , std::vector<Projectile> &projectiles)  {
    for (Enemy &enemy : enemies) {
        if (isInRange(enemy) && currentTime - lastAttackTime >= cadence) {
            lastAttackTime = currentTime;
            projectiles.push_back(Projectile(x, y, 10, power,  enemy.uniqueId));
        }
    }
}

//TYPE PROJETILE DAMAGE
Tower getTowerSlow(int x, int y) {
    return Tower{x, y, 15, 2, 2,250};
}
Tower getTowerShortRange(int x, int y) {
    return Tower{x, y, 9, 1, 1,500};
}
Tower getTowerLongRange(int x, int y) {
    return Tower{x, y, 30, 5, 3,750};
}


int getTowerRange(int cardSelected) {
    switch (cardSelected) {
        case 0:
            return getTowerSlow(0, 0).range;
        case 1:
            return getTowerShortRange(0, 0).range;
        case 2:
            return getTowerLongRange(0, 0).range;
        default:
            return 0;
    }
}