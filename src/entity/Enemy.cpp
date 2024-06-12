#include "Enemy.hpp"

void Enemy::moveX(float dx){
    x += dx;
}
void Enemy::moveY(float dy){
    y += dy;
}

void Enemy::draw_enemy(float scale, float size, GLuint texture) const{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        // glTranslatef(xOrigin, yOrigin, 0);
        glTranslatef(x*scale, -y*scale, 0);
        glBegin(GL_QUADS);
            glTexCoord2d(1,1);
            glVertex2f(0, 0);
            glTexCoord2d(0,1);
            // glVertex2f(tileWidth, 0);
            glVertex2f(size, 0);
            glTexCoord2d(0,0);
            glVertex2f(size, -size);
            glTexCoord2d(1,0);
            glVertex2f(0, -size);
        glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // glPushMatrix();
    // glTranslatef(x*scale, -y*scale, 0);
    // glColor3f(0.80f, 0.0f, 0.9f);
    // glBegin(GL_TRIANGLES);
    // glVertex2f(0, 0);
    // glVertex2f(size, 0);
    // glVertex2f(size / 2, -size);
    // glEnd();
    // glPopMatrix();
}
