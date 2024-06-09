#include "Enemy.hpp"

void Enemy::moveX(float dx){
    x += dx;
}
void Enemy::moveY(float dy){
    y += dy;
}

void Enemy::draw_enemy(float scale, float size) const{
    glPushMatrix();
    glTranslatef(x*scale, -y*scale, 0);
    glColor3f(0.80f, 0.0f, 0.9f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(size, 0);
    glVertex2f(size / 2, -size);
    glEnd();
    glPopMatrix();
}
