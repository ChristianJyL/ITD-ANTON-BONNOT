#include "Projectile.hpp"

bool Projectile::isTargetAlive(const std::vector<Enemy>& enemies) const {
    for (const auto& enemy : enemies) {
        if (enemy.uniqueId == targetId && enemy.hp > 0) {
            return true;
        }
    }
    return false;
}

void Projectile::move(float deltaTime, const std::vector<Enemy>& enemies) {
    if (!isTargetAlive(enemies)) {
        throw std::runtime_error("No valid target for the projectile");
    }
    const Enemy* target = nullptr;
    for (const auto& enemy : enemies) {
        if (enemy.uniqueId == targetId) {
            target = &enemy;
            break;
        }
    }
    if (!target) {
        throw std::runtime_error("No valid target for the projectile");
    }
    // même principe que pour les ennemis
    float length = std::sqrt(std::pow(target->x - x, 2) + std::pow(target->y - y, 2));
    float directionX = (target->x - x) / length;
    float directionY = (target->y - y) / length;

    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;
}

bool Projectile::isArrived(const std::vector<Enemy>& enemies) const {
    const Enemy* target = nullptr;
    for (const auto& enemy : enemies) {
        if (enemy.uniqueId == targetId) {
            target = &enemy;
            break;
        }
    }
    if (!target) {
        return true;  // Cible non valide
    }

    float distance = std::sqrt((target->x - x) * (target->x - x) + (target->y - y) * (target->y - y));
    return distance < 0.9f;
}

void Projectile::draw_projectile(float scale, float size, GLuint texture) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    // glTranslatef(xOrigin, yOrigin, 0);
    glTranslatef(x * scale, -y * scale, 0);
    glBegin(GL_QUADS);
    glTexCoord2d(0,0);
    glVertex2f(0, 0);
    glTexCoord2d(1,0);
    glVertex2f(size, 0);
    glTexCoord2d(1,1);
    glVertex2f(size, -size);
    glTexCoord2d(0,1);
    glVertex2f(0, -size);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
;
    // glPushMatrix();
    // glTranslatef(x * scale, - y * scale, 0);
    // glColor3f(0.0f, 0.0f, 1.0f);
    // glPointSize(size);
    // glBegin(GL_POINTS);
    // glVertex2f(0, 0);
    // glEnd();
    // glPopMatrix();
}

int Projectile::getTypeTower() const { //On retourne le type de la tour en fonction des dégâts
    switch (damage) {
        case 6:
            return 0;
        case 10:
            return 1;
        case 3:
            return 2;
        default:
            return -1;
    }
}
