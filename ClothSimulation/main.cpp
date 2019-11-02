#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Headers/Viewer.h"
#include "Headers/Program.h"
#include "Headers/stb_image.h"

#define WIDTH 600
#define HEIGHT 600

/** Functions **/
void processInput(GLFWwindow *window);

/** Callback functions **/
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/** Global **/
GLFWwindow *window;
Viewer viewer;

int main(int argc, const char * argv[])
{
    /** Prepare for rendering **/
    // Initialize GLFW
    glfwInit();
    // Set OpenGL version number as 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use the core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MacOS is forward compatible
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    /** Create a GLFW window **/
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cloth Simulator", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set the context of this window as the main context of current thread
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD : this should be done before using any openGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        glfwTerminate(); // This line isn't in the official source code, but I think that it should be added here.
        return -1;
    }
    
    /** Register callback functions **/
    // Callback functions should be registered after creating window and before initializing render loop
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /** Redering loop **/
    while (!glfwWindowShouldClose(window))
    {
        /** Check for events **/
        processInput(window);
        
        /** Set background clolor **/
        glClearColor(0.2f, 0.2f, 0.5f, 1.0f); // Set color value (R,G,B,A) - Set Status
        glClear(GL_COLOR_BUFFER_BIT); // Use the color to clear screen - Use Status
        
        /** ---------------------------------- Simulation & Rendering ---------------------------------- **/
        
        
        
        /** ---------------------------------- Simulation & Rendering ---------------------------------- **/
        
        /** Display **/
        glfwSwapBuffers(window);
        glfwPollEvents(); // Update the status of window
    }

    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Reset the size of glViewport
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    /** Keyboard control **/ // If key did not get pressed it will return GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        //
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        //
    }
}
