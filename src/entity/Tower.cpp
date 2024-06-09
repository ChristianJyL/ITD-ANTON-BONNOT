#include "Tower.hpp"

bool Tower::isInRange(const Enemy& enemy) const {
    int distance = std::max(x-enemy.x , y - enemy.y);
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