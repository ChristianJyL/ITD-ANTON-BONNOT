#pragma once

#include <img/img.hpp>
#include <vector>
#include "glad/glad.h"
#include "data.hpp"
#include <iostream>

GLuint loadTexture(uint8_t const* data, int width, int height);
inline GLuint loadTexture(img::Image const& image) { return loadTexture(image.data(), image.width(), image.height()); }

void draw_quad_with_texture(GLuint textureId);

void draw_one_card(GLuint textureId, float x, float y, float width, float height);
void draw_deck(GLuint textureId, float x, float y, float width, float height, int nb_cards);
void draw_grid(float x, float y , float tileWidth, float tileHeight);

void draw_cell(float x, float y, float tileWidth, float tileHeight, GLuint textureId =0);

void draw_map(float x, float y, float tileWidth, float tileHeight, const Data& data);
void draw_grid_available(float x, float y, float tileWidth, float tileHeight, const Data& data);



void draw_hovered_cell(float x, float y, float width, float height);
void draw_hovered_card(float x, float y, float width, float height);