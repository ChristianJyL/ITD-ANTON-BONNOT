#include <glad/glad.h>
#include "other/file.hpp"
#include "other/graph.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "game/App.hpp"

namespace {
    App& window_as_app(GLFWwindow* window)
    {
        return *static_cast<App*>(glfwGetWindowUserPointer(window));
    }
}

// Optional: limit the frame rate
constexpr double TARGET_TIME_FOR_FRAME { 1.0 / 60.0 };

int main() {
/*
    ITD itd;
    std::filesystem::path path {"data/map.itd"};
    if (isValidITD(path,itd)) {
        std::cout << "ITD file is valid" << std::endl;
    } else {
        std::cout << "ITD file is not valid" << std::endl;
    }

    std::cout << getWeight(itd.list_adjacency, 0, 1) << std::endl;

    std::cout << "test" << std::endl;
    Graph::WeightedGraph graph_from_function = Graph::build_from_adjacency_list(itd.list_adjacency);

    graph_from_function.print_DFS(0);


    return 0;*/


    // Set an error callback to display glfw errors
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "Error " << error << ": " << description << std::endl;
    });

    // Initialize glfw
    if (!glfwInit()) {
        return -1;
    }

// Not working on apple with those hint for unknown reason
// #ifdef __APPLE__
//     // We need to explicitly ask for a 3.3 context on Mac
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// #endif

    // Create window
    GLFWwindow* window { glfwCreateWindow(1280, 720, "Window", nullptr, nullptr) };
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    App app {};

    glfwSetWindowUserPointer(window, &app);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        window_as_app(window).key_callback(key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        window_as_app(window).mouse_button_callback(button, action, mods);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        window_as_app(window).scroll_callback(xoffset, yoffset);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        window_as_app(window).cursor_position_callback(xpos, ypos);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        window_as_app(window).size_callback(width, height);
    });
    
    // Force calling the size_callback of the game to set the right viewport and projection matrix
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        app.size_callback(width, height);
    }

    app.setup();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {

        // Get time (in second) at loop beginning
		double startTime { glfwGetTime() };

        app.update();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Optional: limit the frame rate
		double elapsedTime { glfwGetTime() - startTime };
        // wait the remaining time to match the target wanted frame rate
		if(elapsedTime < TARGET_TIME_FOR_FRAME)
		{
			glfwWaitEventsTimeout(TARGET_TIME_FOR_FRAME-elapsedTime);
		}
    }

    glfwTerminate();
    return 0;
}
