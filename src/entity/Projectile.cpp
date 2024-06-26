#include "Projectile.hpp"

bool Projectile::isTargetAlive(const std::vector<Enemy> &enemies) const
{
    for (const auto &enemy : enemies)
    {
        if (enemy.uniqueId == targetId && enemy.hp > 0)
        {
            return true;
        }
    }
    return false;
}

void Projectile::move(float deltaTime, const std::vector<Enemy> &enemies)
{
    if (!isTargetAlive(enemies))
    {
        throw std::runtime_error("No valid target for the projectile");
    }
    const Enemy *target = nullptr;
    for (const auto &enemy : enemies)
    {
        if (enemy.uniqueId == targetId)
        {
            target = &enemy;
            break;
        }
    }
    if (!target)
    {
        throw std::runtime_error("No valid target for the projectile");
    }
    // même principe que pour les ennemis
    float length = std::sqrt(std::pow(target->x - x, 2) + std::pow(target->y - y, 2));
    float directionX = (target->x - x) / length;
    float directionY = (target->y - y) / length;

    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;
}

bool Projectile::isArrived(const std::vector<Enemy> &enemies) const
{
    const Enemy *target = nullptr;
    for (const auto &enemy : enemies)
    {
        if (enemy.uniqueId == targetId)
        {
            target = &enemy;
            break;
        }
    }
    if (!target)
    {
        return true; // Cible non valide
    }

    float distance = std::sqrt((target->x - x) * (target->x - x) + (target->y - y) * (target->y - y));
    return distance < 0.9f;
}

int Projectile::getTypeTower() const
{ // On retourne le type de la tour en fonction des dégâts
    switch (damage){
    case 15:
        return 0;
    case 9:
        return 1;
    case 30:
        return 2;
    default:
        return -1;
    }
}

void Projectile::draw_projectile(float scale, float size, std::unordered_map<std::string, GLuint> textures) const
{
    GLuint texture;
    switch (getTypeTower()){
        case 0 : 
        texture = textures["pizza"];
        break;
        case 1 :
        texture = textures["bucket"];
        break;
        case 2 : 
        texture = textures["burger"];
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    glTranslatef(x * scale, -y * scale, 0);
    glBegin(GL_QUADS);
            glTexCoord2d(1,1);
            glVertex2f(0, 0);
            glTexCoord2d(0,1);
            glVertex2f(size, 0);
            glTexCoord2d(0,0);
            glVertex2f(size, -size);
            glTexCoord2d(1,0);
            glVertex2f(0, -size);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
