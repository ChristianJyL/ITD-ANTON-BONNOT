#include "model.hpp"

#include "glad/glad.h"

GLuint loadTexture(uint8_t const* data, int width, int height) {
    GLuint textureId {};

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // Error on MACOS (segmentation fault) when using glGenerateMipmap
    // glGenerateMipmap(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_2D);

    return textureId;
}

void draw_quad_with_texture(GLuint textureId) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
        int width = 1280;
        int height = 720;
        float aspect_ratio = (float)width / (float)height;
        float x_min = -1.0f * aspect_ratio;
        float y_min = -1.0f * aspect_ratio;
        glTexCoord2d(0,0);
        glVertex2f(x_min, -1);

        glTexCoord2d(1,0);
        glVertex2f(x_min+3.0f, -1);

        glTexCoord2d(1,1);
        glVertex2f(x_min+3.0f, 0.5f);

        glTexCoord2d(0,1);
        glVertex2f(x_min, 0.5f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw_one_card(GLuint textureId, float xOrigin, float yOrigin, float width, float height){
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2f(0, 0);

        glTexCoord2d(1,0);
        glVertex2f(width, 0);

        glTexCoord2d(1,1);
        glVertex2f(width, -height);

        glTexCoord2d(0,1);
        glVertex2f(0, -height);
    glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_TEXTURE_2D);

}

void draw_deck(GLuint textureId, float xOrigin, float yOrigin, float width, float height, int nb_cards){ //mettre un vecteur de GLuint à terme pour les textures
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    float increment = width / nb_cards;
    for(int i = 0; i < nb_cards; i++){
        glColor3f(1.0f, i%2, 1.0f - i * 0.1f);
        draw_one_card(textureId, i * increment, 0, increment, height);
    }
    glPopMatrix();
}

void draw_grid(float xOrigin, float yOrigin, float tileWidth, float tileHeight){ //fonction d'aide pour voir la grille (à revoir parce que valeur brute pour l'instant)
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    for(int i = 0; i < 30; i++){
        glVertex2f(xOrigin + i * tileWidth, yOrigin);
        glVertex2f(xOrigin + i * tileWidth, yOrigin + 1.5);
    }
    for(int i = 0; i < 15; i++){
        glVertex2f(xOrigin, yOrigin + i * tileHeight);
        glVertex2f(xOrigin + 3.0f, yOrigin + i * tileHeight);
    }
    glEnd();
}

void draw_cell(float xOrigin, float yOrigin, float tileWidth, float tileHeight, GLuint textureId){
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
        glTranslatef(xOrigin, yOrigin, 0);
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(tileWidth, 0);
            glVertex2f(tileWidth, -tileHeight);
            glVertex2f(0, -tileHeight);
        glEnd();
    glPopMatrix();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_TEXTURE_2D);
}

void draw_map(float x, float y, float tileWidth, float tileHeight, const Data& data){ //Dessine la map en fonction de la grille de data (remplacer les couleurs avec les textures à terme)
    glPushMatrix();
    glTranslatef(x, y, 0);
    for (int j = data.height -1; j >= 0; --j)
    {
        for (unsigned int i = 0; i < data.width; ++i)
        {
            switch (data.getCell(i,j))
            {
            case 1:
                glColor3f(data.path.r / 255.0f, data.path.g / 255.0f, data.path.b / 255.0f);
                break;
            case 2:
                glColor3f(data.start.r / 255.0f, data.start.g / 255.0f, data.start.b / 255.0f);
                break;
            case 3:
                glColor3f(data.end.r / 255.0f, data.end.g / 255.0f, data.end.b / 255.0f);
                break;
            case 4:
                glColor3f(1.0f, 0.8f, 0.0f);
                break;
            case 5:
                glColor3f(1.0f, 0.5f, 0.0f);
                break;
            case 6:
                glColor3f(1.0f, 0.1f, 0.0f);
                break;
            default:
                glColor3f(0.0f, 0.0f, 0.0f);
                break;
            }
            draw_cell(i * tileWidth, - j * tileHeight, tileWidth, tileHeight);
        }
    }
    glPopMatrix();
}

void draw_grid_available(float xOrigin, float yOrigin, float tileWidth, float tileHeight, const Data& data){
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int j = data.height -1; j >= 0; --j)
    {
        for (unsigned int i = 0; i < data.width; ++i)
        {
            if(data.getCell(i,j) == 0){
                draw_cell( i * tileWidth, -j * tileHeight, tileWidth, tileHeight);
            }
        }
    }
    glPopMatrix();
}

void draw_hovered_cell(float xOrigin, float yOrigin, float width, float height) {
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Épaisseur de la ligne
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    glVertex2f( width, 0);
    glVertex2f( width, - height);
    glVertex2f(0, - height);
    glEnd();
    glPopMatrix();
}

void draw_hovered_card(float xOrigin, float yOrigin, float width, float height){
    glPushMatrix();
        glTranslatef(xOrigin, yOrigin, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2.0f); // Épaisseur de la ligne
        glBegin(GL_LINE_LOOP);
            glVertex2f(0, 0);
            glVertex2f(width, 0);
            glVertex2f(width, - height);
            glVertex2f( 0,  - height);
        glEnd();
    glPopMatrix();
}

void draw_enemies(float xOrigin, double yOrigin, std::vector<Enemy> enemies, float size){
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    for(Enemy const& enemy : enemies){
        //std::cout << "x: " << enemy.x << " y: " << enemy.y << std::endl;
        enemy.draw_enemy(0.1 , size);
    }
    glPopMatrix();
}


void draw_projectiles(float xOrigin, float yOrigin, const std::vector<Projectile>& projectiles, float size){
    glPushMatrix();
    glTranslatef(xOrigin, yOrigin, 0);
    for(Projectile const& projectile : projectiles){
        projectile.draw_projectile(0.1, size);
    }
    glPopMatrix();
}



