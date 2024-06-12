#include "Projectile.hpp"

void Projectile::move(float deltaTime)
{
    if (target == nullptr)
    {
        throw std::runtime_error("No target for the projectile");
    }
    // même principe que pour les ennemis
    float length = std::sqrt(std::pow(target->x - x, 2) + std::pow(target->y - y, 2));
    float directionX = (target->x - x) / length;
    float directionY = (target->y - y) / length;

    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;
}

bool Projectile::isArrived() const
{
    if (target == nullptr)
    {
        throw std::runtime_error("No target for the projectile");
    }
    // même principe que pour les ennemis
    float distance = std::sqrt((target->x - x) * (target->x - x) + (target->y - y) * (target->y - y));
    return distance < 0.3f;
}

void Projectile::draw_projectile(float scale, float size, GLuint texture) const
{
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

    // glPushMatrix();
    // glTranslatef(x * scale, - y * scale, 0);
    // glColor3f(0.0f, 0.0f, 1.0f);
    // glPointSize(size);
    // glBegin(GL_POINTS);
    // glVertex2f(0, 0);
    // glEnd();
    // glPopMatrix();
}
