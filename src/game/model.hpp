#pragma once

#include <img/img.hpp>
#include <vector>
#include "glad/glad.h"
#include "other/utils.hpp"
#include "data.hpp"
#include <iostream>

GLuint loadTexture(uint8_t const* data, int width, int height);
inline GLuint loadTexture(img::Image const& image) { return loadTexture(image.data(), image.width(), image.height()); }

void draw_quad_with_texture(GLuint textureId);

void draw_one_card(std::unordered_map<int, GLuint> cards, float x, float y, float width, float height);
void draw_deck(std::unordered_map<int, GLuint> cards, float x, float y, float width, float height, int nb_cards);
void draw_grid(float x, float y , float tileWidth, float tileHeight);

void draw_cell(float x, float y, float tileWidth, float tileHeight, GLuint textureId =0);

void draw_map(float x, float y, float tileWidth, float tileHeight, const Data& data, std::unordered_map<std::string, GLuint> textures);
void draw_grid_available(float x, float y, float tileWidth, float tileHeight, const Data& data);

void draw_hovered_cell(float x, float y, float width, float height);
void draw_hovered_card(float x, float y, float width, float height);
void draw_hovered_tower(float xOrigin, float yOrigin, float width, float height , int range, GLuint texture);

void draw_enemies(float xOrigin, double yOrigin, std::vector<Enemy> enemies, float size, GLuint texture);
void draw_projectiles(float xOrigin, float yOrigin, const std::vector<Projectile>& projectiles, float size, std::unordered_map<std::string, GLuint> textures);
void draw_menu(GLuint textureId, float xOrigin, float yOrigin, float width, float height);
void draw_button(float xOrigin, float yOrigin, float width, float tileHeight, GLuint textureId, bool isPaused);