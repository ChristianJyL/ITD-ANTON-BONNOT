#include "model.hpp"

#include "glad/glad.h"
#include <iostream>



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
        glTexCoord2d(0,0);
        //calcul x min with aspect ratio...
        int width = 1280;
        int height = 720;
        float aspect_ratio = (float)width / (float)height;
        float x_min = -1.0f * aspect_ratio;
        float y_min = -1.0f * aspect_ratio;
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
        glVertex2f(x+width, y+height);

        glTexCoord2d(0,1);
        glVertex2f(x, y+height);
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
