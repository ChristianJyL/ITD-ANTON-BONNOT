#include "App.hpp"
#include "data.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <img/img.hpp>
#include <iostream>
#include "simpletext.h"
#include "other/utils.hpp"
#include "view.hpp"
#include <string>

static const float TILE_WIDTH = 0.1f;
static const float TILE_HEIGHT = 0.1f;
static const int NB_CARDS = 3;

App::App() : _previousTime(0.0), _viewSize(2.0), _mouseX(0.0f), _mouseY(0.0f)
{
    // load what needs to be loaded here (for example textures)
    data.loadFromITD("data/map2.itd");
    if (!data.isEverythingValid())
    {
        throw std::runtime_error{"Something went wrong with the ITD file"};
    }
    data.putShortestPaths();

    // Chargement des textures
    textures["floor"] = loadTexture(img::load(make_absolute_path("images/floor.png", true), 4, true));
    textures["manager"] = loadTexture(img::load(make_absolute_path("images/manager.png", true), 4, true));
    textures["carpet"] = loadTexture(img::load(make_absolute_path("images/carpet.PNG", true), 4, true));
    textures["towerSlow"] = loadTexture(img::load(make_absolute_path("images/towerSlow.png", true), 4, true));
    textures["towerShort"] = loadTexture(img::load(make_absolute_path("images/towerShort.png", true), 4, true));
    textures["towerLong"] = loadTexture(img::load(make_absolute_path("images/towerLong.png", true), 4, true));
    textures["pizza"] = loadTexture(img::load(make_absolute_path("images/pizza.png", true), 4, true));
    textures["bucket"] = loadTexture(img::load(make_absolute_path("images/bucket.png", true), 4, true));
    textures["burger"] = loadTexture(img::load(make_absolute_path("images/burger.png", true), 4, true));
    textures["costumer"] = loadTexture(img::load(make_absolute_path("images/Costumer.png", true), 4, true));
    textures["costumer_green"] = loadTexture(img::load(make_absolute_path("images/Costumer_green.png", true), 4, true));
    textures["homepage"] = loadTexture(img::load(make_absolute_path("images/homepage.png", true), 4, true));
    textures["wasted"] = loadTexture(img::load(make_absolute_path("images/wasted.png", true), 4, true));
    textures["victory"] = loadTexture(img::load(make_absolute_path("images/victory.png", true), 4, true));
    textures["tile"] = loadTexture(img::load(make_absolute_path("images/tile.png", true), 4, true));
    textures["menu"] = loadTexture(img::load(make_absolute_path("images/menu.png", true), 4, true));
    textures["pause"] = loadTexture(img::load(make_absolute_path("images/pause.png", true), 4, true));
    textures["play"] = loadTexture(img::load(make_absolute_path("images/play.png", true), 4, true));

    cards[0] = loadTexture(img::load(make_absolute_path("images/pizzaMenu.png", true), 4, true));
    cards[1] = loadTexture(img::load(make_absolute_path("images/bucketMenu.png", true), 4, true));
    cards[2] = loadTexture(img::load(make_absolute_path("images/burgerMenu.png", true), 4, true));


    _mapWidth = static_cast<float>(data.width) / 10.0f;
    _mapHeight = static_cast<float>(data.height) / 10.0f;

}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(239/255.0f, 234/255.0f, 213/255.0f , 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
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

    switch (gameState)
    {
    case GameState::MainMenu:
        renderMainMenu();
        break;
    case GameState::InGame:
        if (!isPaused){
            data.alternateSpawn(currentTime);
            data.moveEnemies(elapsedTime);
            data.moveProjectiles(elapsedTime);
            data.attackEnemies(currentTime);

            // if all enemies are dead (après 20 secondes)
            if (data.enemies.empty() && data.waveCount >= TOTAL_WAVES)
            {
                gameState = GameState::EndScreen;
            }
            if (!data.isAlive)
            {
                gameState = GameState::EndScreen;
            }

            render();
            break;
        }  else {
            render();
            break;
        }
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

    draw_map(_xMin, 0.5f, TILE_WIDTH, TILE_HEIGHT, data, textures);
    draw_enemies(_xMin, _mapHeight - 1, data.enemies, 0.1f, textures["costumer"] , textures["costumer_green"]);

    draw_projectiles(_xMin, _mapHeight - 1, data.projectiles, 0.08f, textures);

    if (data.isCardSelected())
    {
        draw_grid_available(_xMin, 0.5f, TILE_WIDTH, TILE_HEIGHT, data);
    }

    if (_mouseX >= _xMin && _mouseX < _xMin + _mapWidth && _mouseY >= -1 && _mouseY < 0.5)
    {
        int tileX = static_cast<int> ((_mouseX - _xMin) / TILE_WIDTH);
        int tileY =  static_cast<int> (-(_mouseY - 0.5f) / TILE_HEIGHT);
        draw_hovered_cell(_xMin + (float) tileX * TILE_WIDTH, 0.5f -  (float) tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

        if (data.isCardSelected())
        {
            int Range = getTowerRange(data.cardSelected);
            draw_hovered_tower(_xMin + (float) tileX * TILE_WIDTH, 0.5f - (float) tileY * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, Range, textures["towerSlow"]);
        }
    }

    draw_deck(cards, _xMin, 1.0f, _mapWidth, _mapHeight - 1, NB_CARDS);

    if (_mouseX >= _xMin && _mouseX < _xMin + _mapWidth && _mouseY >= 0.5 && _mouseY < 1)
    {
        int cardX = static_cast<int> ((_mouseX - _xMin) / (3.0f / NB_CARDS));
        draw_hovered_card(_xMin + (float) cardX * (3.0f / NB_CARDS), 1.0f, 3.0f / NB_CARDS, 0.5f);
    }

    if (data.isCardSelected())
    {
        draw_hovered_card(_xMin + (float) data.cardSelected * (3.0f / NB_CARDS), 1.0f, 3.0f / NB_CARDS, 0.5f);
    }

    draw_menu(textures["menu"], _xMin + _mapWidth, 1, 0.55f, 2);

    // Gestion du bouton de pause

    if (isPaused)
    {
        draw_cell(_xMin + _mapWidth + 0.175, -0.15f, 2 * TILE_WIDTH, 2 * TILE_HEIGHT, textures["play"]);
    } else {
        draw_cell(_xMin + _mapWidth + 0.175, -0.15f, 2 * TILE_WIDTH, 2 * TILE_HEIGHT, textures["pause"]);
    }
    display_money(data.money);
    display_score(data.waveCount);
    display_warning(data.life);
}

void App::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (gameState == GameState::MainMenu && key == GLFW_KEY_ENTER) {
            gameState = GameState::InGame;
        } else if (gameState == GameState::EndScreen && key == GLFW_KEY_ENTER) {
            glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
        } else if (gameState == GameState::InGame && key == GLFW_KEY_SPACE) {
            if (isPaused) {
                isPaused = false;
            } else {
                isPaused = true;
            }
        }

    }
}

void App::mouse_button_callback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && gameState == GameState::InGame) {
        // Le bouton gauche de la souris a été pressé
        double xpos, ypos;
        // Récupère la position actuelle du curseur
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        float x =  (xpos - _width / 2.0f) * 2.0f / _height;
        float y = 1.0f -ypos  / (_height / 2.0);

        //std::cout << "x: " << x << " y: " << y << std::endl;

        // Détermine la partie de la fenêtre sur laquelle l'utilisateur a cliqué
        if (x >= _xMin && x < _xMin + _mapWidth && y >= -1 && y < _mapHeight - 1) { // Clique sur la map
            if (!data.isCardSelected()) {
                /*
                int tileX = static_cast<int>((x - _xMin) / TILE_WIDTH);
                int tileY = static_cast<int>(-(y - (_mapHeight - 1)) / TILE_HEIGHT);
                std::cout << "Tile clicked: (" << tileX << ", " << tileY << ")" << std::endl;*/
                data.unselectCard();
            } else {
                // Placement de la tour sélectionnée
                int tileX = static_cast<int>((x - _xMin) / TILE_WIDTH);
                int tileY = static_cast<int>(-(y - (_mapHeight - 1)) / TILE_HEIGHT);
                //std::cout << "Tile clicked: (" << tileX << ", " << tileY << ")" << std::endl;
                data.placeCard(tileX, tileY);
                data.unselectCard();
            }
        } else if (x >= _xMin && x < _xMin + _mapWidth && y >= _mapHeight - 1 && y < 1) { // Clique sur le deck
            int cardX = static_cast<int>((x - _xMin) / (_mapWidth /NB_CARDS)); // On récupère la carte cliquée
            data.selectCard(cardX);
            //std::cout << "Card clicked: " << data.cardSelected << std::endl;
        } else { // Clique sur le menu/info
            if ( x >= _xMin + _mapWidth + 0.175 && x < _xMin + _mapWidth + 0.175 + 2 * TILE_WIDTH && y <= -0.15 && y > -0.15 - 2 * TILE_HEIGHT) {
                if (isPaused) {
                    isPaused = false;
                } else {
                    isPaused = true;
                }
            } else if ( x >= _xMin + _mapWidth + 0.183 && x < _xMin + _mapWidth + 0.183 + 2 * TILE_WIDTH && y <= -0.53 &&  y > -0.54 - 1.5* TILE_HEIGHT) {
                glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
            }
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

void App::renderMainMenu()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures["homepage"]);
    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0);
    glBegin(GL_QUADS);
    glTexCoord2d(1, 1);
    glVertex2f(2.3f, 1.5f);
    glTexCoord2d(0, 1);
    glVertex2f(-1.3f, 1.5f);
    glTexCoord2d(0, 0);
    glVertex2f(-1.3f, -0.5f);
    glTexCoord2d(1, 0);
    glVertex2f(2.3f, -0.5f);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void App::renderGameOver()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures["wasted"]);
    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0);
    glBegin(GL_QUADS);
    glTexCoord2d(1, 1);
    glVertex2f(2.3f, 1.5f);
    glTexCoord2d(0, 1);
    glVertex2f(-1.3f, 1.5f);
    glTexCoord2d(0, 0);
    glVertex2f(-1.3f, -0.5f);
    glTexCoord2d(1, 0);
    glVertex2f(2.3f, -0.5f);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // Affichage du score
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.Label("Fired at round :", static_cast<int>(_width * 0.5), static_cast<int>(_height * 0.7), SimpleText::CENTER);
    std::string scoreStr = std::to_string(data.waveCount);
    TextRenderer.Label(scoreStr.c_str(), static_cast<int>(_width * 0.5), static_cast<int>(_height * 0.72), SimpleText::CENTER);
    TextRenderer.Label("Press Enter to Quit", static_cast<int>(_width * 0.5), static_cast<int>(_height * 0.90), SimpleText::CENTER);
    TextRenderer.Render();
}

void App::renderWin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, textures["victory"]);
    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0);
    glBegin(GL_QUADS);
    glTexCoord2d(1, 1);
    glVertex2f(2.3f, 1.5f);
    glTexCoord2d(0, 1);
    glVertex2f(-1.3f, 1.5f);
    glTexCoord2d(0, 0);
    glVertex2f(-1.3f, -0.5f);
    glTexCoord2d(1, 0);
    glVertex2f(2.3f, -0.5f);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    TextRenderer.SetColorf(SimpleText::TEXT_COLOR, 241/255.0f, 196/255.0f, 15/255.0f, 1);
    TextRenderer.Label("Press Enter to Quit", static_cast<int>(_width * 0.5), static_cast<int>(_height * 0.98), SimpleText::CENTER);
    TextRenderer.Render();
}

void App::display_money(int money) {
    std::string moneyStr = std::to_string(money);
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.Label(moneyStr.c_str(), static_cast<int>(_width * 0.92) , static_cast<int>(_height * 0.2), SimpleText::CENTER);
    TextRenderer.Render();
}

void App::display_warning(int life) {
    std::string warningStr = std::to_string(std::abs(life-3));
    std::string message = "Warning: " + warningStr + " (max 3)";
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::RED);
    TextRenderer.Label(message.c_str(), static_cast<int>(_width * 0.922), static_cast<int>(_height * 0.3), SimpleText::CENTER);
    TextRenderer.Render();
}

void App::display_score(int score) {
    std::string scoreStr = std::to_string(score);
    TextRenderer.Label(scoreStr.c_str(), static_cast<int>(_width * 0.92), static_cast<int>(_height * 0.45), SimpleText::CENTER);
    TextRenderer.Render();
}

void App::free_textures() {
    for (auto& texture : textures) {
        glDeleteTextures(1, &texture.second);
    }
    for (auto& card : cards) {
        glDeleteTextures(1, &card.second);
    }
}