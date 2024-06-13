#pragma once

#include "glad/glad.h"
#include <simpletext.h>
#include "data.hpp"

class App {
public:
    enum class GameState {
        MainMenu,
        InGame,
        EndScreen
    };

    App();

    void setup();
    void update();
    
    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);
    void renderMainMenu();
    void renderGameOver();
    void renderWin();
    void display_money(int money);

private:
    void render();
    GameState gameState = GameState::MainMenu;
    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    float _aspectRatio {};
    float _xMin {};
    float _mapWidth {};
    float _mapHeight {};

    Data data;
    float _mouseX, _mouseY;

    // Add your variables here
    GLuint _texture {};
    std::unordered_map<std::string, GLuint> textures {};
    std::unordered_map<int, GLuint> cards {};
    float _angle {};

    SimpleText TextRenderer {};
};