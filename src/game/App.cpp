#include "App.hpp"
#include "data.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <img/img.hpp>

#include <sstream>
#include <iostream>

#include "simpletext.h"
#include "other/utils.hpp"
#include "model.hpp"

static const float TILE_WIDTH = 0.1;
static const float TILE_HEIGHT = 0.1;
static const int NB_CARDS = 3;

std::unordered_map<std::string, GLuint> addTexture(std::unordered_map<std::string, GLuint> textures, std::string id, std::string image)
{
    textures[id] = loadTexture(img::load(make_absolute_path(image, true), 4, true));
    return textures;
}

App::App() : _previousTime(0.0), _viewSize(2.0), _mouseX(0.0f), _mouseY(0.0f)
{
    // load what needs to be loaded here (for example textures)
    img::Image image_map = {img::load(make_absolute_path("images/map3.png", true), 3, true)};
    img::Image image_deck = {img::load(make_absolute_path("images/level.png", true), 3, true)};
    // img::Image wood_floor = {img::load(make_absolute_path("images/wood_floor.png", true), 3, true)};
    data.loadFromITD("data/map2.itd");
    data.putShortestPaths();

    _texture = loadTexture(image_map);
    textures = addTexture(textures, "floor", "images/wood_floor.png");
    textures = addTexture(textures, "carpet", "images/carpet.PNG");
    textures = addTexture(textures, "manager", "images/manager.png");
    textures = addTexture(textures, "towerSlow", "images/towerSlow.png");
    textures = addTexture(textures, "towerShort", "images/towerShort.png");
    textures = addTexture(textures, "towerLong", "images/towerLong.png");
    // textures = addTexture(textures, "costumer", "images/costumer.png");
    textures = addTexture(textures, "food", "images/food.png");
    textures["costumer"] = loadTexture(img::load(make_absolute_path("images/costumer.png", true), 4, true));

    // _woodTexture = loadTexture(wood_floor);
    _mapWidth = static_cast<float>(image_map.width()) / 10.0f;
    _mapHeight = static_cast<float>(image_map.height()) / 10.0f;
    // data.addEnemy({13,4,0,1,0});    data.addEnemy({13,4,0,2,0});    data.addEnemy({13,4,0,1.5,0});    data.addEnemy({13,4,0,0.5f,0});

    // Afficher grid
    std::cout << "Grid: " << std::endl;
    data.printGrid();

    // Afficher coordNodes
    std::cout << "Coordonnée nodes: " << std::endl;
    for (const auto &pair : data.coordNodes)
    {
        std::cout << "Key: " << pair.first << " Value: (" << pair.second.first << ", " << pair.second.second << ")" << std::endl;
    }

    // Afficher graph
    std::cout << "Graph: " << std::endl;
    data.graph.print_DFS(0);

    std::cout << "Is everything valid: " << data.isEverythingValid() << std::endl;
}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);
    _aspectRatio = (float)_width / (float)_height;
    _xMin = -1.0f * _aspectRatio;
}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;

    switch (gameState) {
        case GameState::MainMenu:
            renderMainMenu();
            break;
        case GameState::InGame:
            data.alternateSpawn(currentTime);
            data.moveEnemies(elapsedTime);
            data.moveProjectiles(elapsedTime);
            data.attackEnemies(currentTime);

            //if all enemies are dead (après 20 secondes)
            if (data.enemies.empty() && currentTime > 20)
            {
                gameState = GameState::EndScreen;
            }
            if (!data.isAlive)
            {
                gameState = GameState::EndScreen;
            }

            render();
            break;
        case GameState::EndScreen:
            if (data.isAlive)
            {
                renderWin();
            }
            else
            {
                renderGameOver();
            }
            break;
    }

}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw_quad_with_texture(_texture);

    draw_map(_xMin, 0.5f, TILE_WIDTH, TILE_HEIGHT, data, textures);
    draw_enemies(_xMin, _mapHeight - 1, data.enemies, 0.1, textures["costumer"]);

    draw_projectiles(_xMin, _mapHeight - 1, data.projectiles, 0.1, textures["food"]);

    if (data.isCardSelected())
    {
        draw_grid_available(_xMin, 0.5f, TILE_WIDTH, TILE_HEIGHT, data);
    }

    // draw_grid(_xMin, -1, TILE_WIDTH, TILE_HEIGHT);

    if (_mouseX >= _xMin && _mouseX < _xMin + _mapWidth && _mouseY >= -1 && _mouseY < 0.5)
    {
        int tileX = (_mouseX - _xMin) / TILE_WIDTH;
        int tileY = -(_mouseY - 0.5f) / TILE_HEIGHT;
        draw_hovered_cell(_xMin + tileX * TILE_WIDTH, 0.5f - tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

        if (data.isCardSelected())
        {
            int Range = getTowerRange(data.cardSelected);
            draw_hovered_tower(_xMin + tileX * TILE_WIDTH, 0.5f - tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, Range);
        }
    }

    draw_deck(_texture, _xMin, 1.0f, _mapWidth, _mapHeight - 1, NB_CARDS);

    if (_mouseX >= _xMin && _mouseX < _xMin + _mapWidth && _mouseY >= 0.5 && _mouseY < 1)
    {
        int cardX = (_mouseX - _xMin) / (3.0f / NB_CARDS);
        draw_hovered_card(_xMin + cardX * (3.0f / NB_CARDS), 1.0f, 3.0f / NB_CARDS, 0.5f);
    }

    if (data.isCardSelected())
    {
        draw_hovered_card(_xMin + data.cardSelected * (3.0f / NB_CARDS), 1.0f, 3.0f / NB_CARDS, 0.5f);
    }

}

void App::key_callback(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (gameState == GameState::MainMenu && key == GLFW_KEY_ENTER) {
            gameState = GameState::InGame;
        } else if (gameState == GameState::EndScreen && key == GLFW_KEY_ENTER) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
        }
    }
}

void App::mouse_button_callback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && gameState == GameState::InGame)
    {
        // Le bouton gauche de la souris a été pressé
        double xpos, ypos;
        // Récupère la position actuelle du curseur
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        float x = (xpos - _width / 2) * 2.0f / _height;
        float y = 1 - ypos / (_height / 2);

        std::cout << "x: " << x << " y: " << y << std::endl;

        // Détermine la partie de la fenêtre sur laquelle l'utilisateur a cliqué
        if (x >= _xMin && x < _xMin + _mapWidth && y >= -1 && y < _mapHeight - 1)
        { // Clique sur la map
            if (!data.isCardSelected())
            {
                // Calculer la tuile sur laquelle l'utilisateur a cliqué
                int tileX = static_cast<int>((x - _xMin) / TILE_WIDTH);
                int tileY = static_cast<int>(-(y - (_mapHeight - 1)) / TILE_HEIGHT);
                std::cout << "Tile clicked: (" << tileX << ", " << tileY << ")" << std::endl;
                data.unselectCard();
            }
            else
            {
                // Placement de la tour selectionnée
                int tileX = static_cast<int>((x - _xMin) / TILE_WIDTH);
                int tileY = static_cast<int>(-(y - (_mapHeight - 1)) / TILE_HEIGHT);
                std::cout << "Tile clicked: (" << tileX << ", " << tileY << ")" << std::endl;
                if (data.placeCard(tileX, tileY))
                {
                    std::cout << "Card placed" << std::endl;
                }
                else
                {
                    std::cout << "Card not placed" << std::endl;
                }
                data.unselectCard();
            }
        }
        else if (x >= _xMin && x < _xMin + _mapWidth && y >= _mapHeight - 1 && y < 1)
        { // Clique sur le deck
            std::cout << "Card" << std::endl;
            // Calculer la carte sur laquelle l'utilisateur a cliqué
            int cardX = static_cast<int>((x - _xMin) / (_mapWidth / NB_CARDS)); // a voir pour utiliser une variable pour avoir quelque chose de variable
            data.selectCard(cardX);
            std::cout << "Card clicked: " << data.cardSelected << std::endl;
        }
        else
        { // Clique sur le menu/info (avec un else pour l'instant, à voir si on change)
            std::cout << "menu/info" << std::endl;
            data.unselectCard();
        }
    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double xpos, double ypos)
{
    _mouseX = (xpos - _width / 2) * 2.0f / _height;
    _mouseY = 1 - ypos / (_height / 2);
}

void App::size_callback(int width, int height)
{
    _width = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio{_width / (float)_height};

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1.0f)
    {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}

// <<<<<<< Updated upstream


void App::renderMainMenu(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    TextRenderer.Label("Main Menu", 640, 320, SimpleText::CENTER);
    TextRenderer.Label("Press Enter to Start", 643, 360, SimpleText::CENTER);
    TextRenderer.Render();
}

void App::renderGameOver(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    TextRenderer.Label("Game Over", 640, 320, SimpleText::CENTER);
    TextRenderer.Label("Press Enter to close the game", 643, 360, SimpleText::CENTER);
    TextRenderer.Render();
}

void App::renderWin(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    TextRenderer.Label("You Win!", 640, 320, SimpleText::CENTER);
    TextRenderer.Label("Press Enter to close the game", 643, 320, SimpleText::CENTER);
    TextRenderer.Render();
}


// =======
bool App::isRunning() {
    return data.isAlive;
}
>>>>>>> Stashed changes
