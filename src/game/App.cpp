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

static const float tileWidth = 0.1;
static const float tileHeight = 0.1;



App::App() : _previousTime(0.0), _viewSize(2.0) {
    // load what needs to be loaded here (for example textures)

    Data data;
    img::Image image_map = {img::load(make_absolute_path("images/map2.png", true), 3, true)};
    img::Image image_deck = {img::load(make_absolute_path("images/level.png", true), 3, true)};
    data.loadFromITD("data/map.itd");
    _texture = loadTexture(image_map);


    // Afficher grid
    std::cout<<"Grid: " << std::endl;
    data.printGrid();

    // Afficher coordNodes
    std::cout<<"Coordonnée nodes: " << std::endl;
    for (const auto& pair : data.coordNodes) {
        std::cout << "Key: " << pair.first << " Value: (" << pair.second.first << ", " << pair.second.second << ")" << std::endl;
    }

    // Afficher graph
    std::cout << "Graph: " << std::endl;
    data.graph.print_DFS(0);

    std::cout << "Is everything valid: " << data.isEverythingValid() << std::endl;


}

void App::setup() {
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);
    _aspectRatio = (float)_width / (float)_height;
}

void App::update() {

    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    _angle += 10.0f * elapsedTime;
    // _angle = std::fmod(_angle, 360.0f);

    render();
}

void App::render() {
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    static float x_min = -1.0f * _aspectRatio;

    draw_quad_with_texture(_texture);
    draw_deck(_texture, x_min, 0.5f, 3.0f, 0.5f, 5);




    //

    // Without set precision
    // const std::string angle_label_text { "Angle: " + std::to_string(_angle) };
    // With c++20 you can use std::format
    // const std::string angle_label_text { std::format("Angle: {:.2f}", _angle) };

    // Using stringstream to format the string with fixed precision
    // std::string angle_label_text {};
    // std::stringstream stream {};
    // stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    // angle_label_text = stream.str();

    // TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);

    // TextRenderer.Render();
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {
}

void App::mouse_button_callback(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Le bouton gauche de la souris a été pressé
        double xpos, ypos;
        // Récupère la position actuelle du curseur
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
        float x =  (xpos - _width / 2) * 2.0f / _height;
        float y = 1 - ypos / (_height / 2);

        std::cout << "x: " << x << " y: " << y << std::endl;
        float x_min = -1.0f * _aspectRatio;
        // Détermine la partie de la fenêtre sur laquelle l'utilisateur a cliqué
        if (x >= x_min && x < x_min+3.0 && y >= -1 && y < 0.5) { //Clique sur la map
            //Une tuile fait 0.1 de largeur et 0.1 de hauteur
            //Calculer la tuile sur laquelle l'utilisateur a cliqué
            int tileX = (x - x_min) / tileWidth;
            int tileY = -(y - 0.5f) / tileHeight;
            std::cout << "Tile clicked: (" << tileX << ", " << tileY << ")" << std::endl;
        } else if (x >= x_min && x < x_min+3.0 && y >= 0.5 && y < 1) { //Clique sur le deck
            std::cout << "Card" << std::endl;
        } else { //Clique sur le menu/info (avec un else pour l'instant, à voir si on change)
            std::cout << "menu/info" << std::endl;
        }

    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/) {
}

void App::size_callback(int width, int height) {
    _width  = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio { _width / (float) _height };

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(aspectRatio > 1.0f) {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}

