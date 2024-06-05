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

void draw_one_card(GLuint textureId, float x, float y, float width, float height){
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, textureId);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2f(x, y);

        glTexCoord2d(1,0);
        glVertex2f(x+width, y);

        glTexCoord2d(1,1);
        glVertex2f(x+width, y-height);

        glTexCoord2d(0,1);
        glVertex2f(x, y-height);
    glEnd();
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDisable(GL_TEXTURE_2D);

}

void draw_deck(GLuint textureId, float x, float y, float width, float height, int nb_cards){ //mettre un vecteur de GLuint à terme pour les textures
    float increment = width / nb_cards;
    float card_width = width / nb_cards;
    for(int i = 0; i < nb_cards; i++){
        glColor3f(1.0f, i%2, 1.0f - i * 0.1f);
        draw_one_card(textureId, x + i * increment, y, card_width, height);
    }
}

void draw_grid(float x, float y, float tileWidth, float tileHeight){ //fonction d'aide pour voir la grille (à revoir parce que valeur brute pour l'instant)
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    for(int i = 0; i < 30; i++){
        glVertex2f(x + i * tileWidth, y);
        glVertex2f(x + i * tileWidth, y + 1.5);
    }
    for(int i = 0; i < 15; i++){
        glVertex2f(x, y + i * tileHeight);
        glVertex2f(x + 3.0f, y + i * tileHeight);
    }
    glEnd();
}

void draw_cell_available(float x, float y, float tileWidth, float tileHeight){
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + tileWidth, y);
        glVertex2f(x + tileWidth, y - tileHeight);
        glVertex2f(x, y - tileHeight);
    glEnd();
}

void draw_grid_available(float x, float y, float tileWidth, float tileHeight, Data data){
    for (int j = data.height -1; j >= 0; --j)
    {
        for (unsigned int i = 0; i < data.width; ++i)
        {
            if(data.getCell(i,j) == 0){
                draw_cell_available(x + i * tileWidth, y - j * tileHeight, tileWidth, tileHeight);
            }
        }
    }
}

void draw_hovered_cell(float x, float y, float width, float height) {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Épaisseur de la ligne
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
}

void draw_hovered_card(float x, float y, float width, float height){
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f); // Épaisseur de la ligne
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
}