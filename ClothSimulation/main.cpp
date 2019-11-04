#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Headers/stb_image.h"
#include "Headers/Cloth.h"
#include "Headers/Program.h"
#include "Headers/Display.h"

#define WIDTH 600
#define HEIGHT 600

#define AIR_FRICTION 0.02
#define TIME_STEP 0.01

/** Functions **/
void processInput(GLFWwindow *window);

/** Callback functions **/
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/** Global **/
GLFWwindow *window;
Cloth cloth(7, 7);
Vec3 ground(0.0, -6.5, 0.0);
Vec3 gravity(0.0, -9.8 / cloth.iterationFreq, 0.0);

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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cloth Simulation", NULL, NULL);
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
    
    ClothRender clothRender(&cloth);
    Vec3 initForce(10.0, 10.0, 10.0);
    cloth.addForce(initForce);
    
    glEnable(GL_DEPTH_TEST);
    
    /** Redering loop **/
    while (!glfwWindowShouldClose(window))
    {
        /** Check for events **/
        processInput(window);
        
        /** Set background clolor **/
        glClearColor(0.69f, 0.77f, 0.87f, 1.0f); // Set color value (R,G,B,A) - Set Status
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /** -------------------------------- Simulation & Rendering -------------------------------- **/
        
        for (int i = 0; i < cloth.iterationFreq; i ++) {
            cloth.computeForce(TIME_STEP, gravity);
            cloth.integrate(AIR_FRICTION, TIME_STEP);
            cloth.collision_response(ground);
        }
        cloth.computeNormal();
        
        clothRender.flush();
        
        /** -------------------------------- Simulation & Rendering -------------------------------- **/
        
        /** Display **/
        glfwSwapBuffers(window);
        glfwPollEvents(); // Update the status of window
    }

    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
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
    
    /** Camera control : [W] [S] [A] [D] **/
    float camSpeed = 0.03f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.pos += cam.front * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.pos -= cam.front * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.pos -= glm::normalize(glm::cross(cam.front, cam.up)) * camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.pos += glm::normalize(glm::cross(cam.front, cam.up)) * camSpeed;
    }
    
    /** Pull cloth **/
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cloth.addForce(Vec3(0.0, 0.0, 10.0));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cloth.addForce(Vec3(0.0, 0.0, -10.0));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cloth.addForce(Vec3(-10.0, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cloth.addForce(Vec3(10.0, 0.0, 0.0));
    }
}
