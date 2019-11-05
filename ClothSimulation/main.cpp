#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Headers/stb_image.h"
#include "Headers/Cloth.h"
#include "Headers/Rigid.h"
#include "Headers/Program.h"
#include "Headers/Display.h"

#define WIDTH 800
#define HEIGHT 800

#define AIR_FRICTION 0.02
#define TIME_STEP 0.01

/** Functions **/
void processInput(GLFWwindow *window);

/** Callback functions **/
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

/** Global **/
GLFWwindow *window;

Vec3 clothPos(-3, 7, -3);
Vec2 clothSize(6, 6);
Cloth cloth(clothPos, clothSize);

Vec3 groundPos(-10, 1.5, -10);
Vec2 groundSize(20, 20);
glm::vec4 groundColor(1.0, 1.0, 1.0, 1.0);
Ground ground(groundPos, groundSize, groundColor);

Vec3 ballPos(0, 2, -3);
int ballRadius = 1;
glm::vec4 ballColor(1.0f, 0.0f, 1.0f, 1.0f);
Ball ball(ballPos, ballRadius, ballColor);

Vec3 gravity(0.0, -9.8 / cloth.iterationFreq, 0.0);
Vec3 bgColor = Vec3(50.0/255, 50.0/255, 50.0/255);

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
    BallRender ballRender(&ball);
    
    Vec3 initForce(10.0, 10.0, 10.0);
    cloth.addForce(initForce);
    
    glEnable(GL_DEPTH_TEST);
    glPointSize(3);
    
    /** Redering loop **/
    while (!glfwWindowShouldClose(window))
    {
        /** Check for events **/
        processInput(window);
        
        /** Set background clolor **/
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0); // Set color value (R,G,B,A) - Set Status
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /** -------------------------------- Simulation & Rendering -------------------------------- **/
        
        for (int i = 0; i < cloth.iterationFreq; i ++) {
            cloth.computeForce(TIME_STEP, gravity);
            cloth.integrate(AIR_FRICTION, TIME_STEP);
            cloth.collision_response(&ground, &ball);
        }
        cloth.computeNormal();
        
        clothRender.flush();
        ballRender.flush();
        
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
    
    /** Set draw mode **/
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        drawMode = DRAW_NODES;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        drawMode = DRAW_LINES;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        drawMode = DRAW_FACES;
    }
    
    /** Drop the cloth **/
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            cloth.unPin(cloth.pin1);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        cloth.unPin(cloth.pin2);
    }
    
    /** Camera control : [W] [S] [A] [D] [Q] [E] **/
    float camSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.pos.y += camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.pos.y -= camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.pos.x -= camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.pos.x += camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cam.pos.z -= camSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cam.pos.z += camSpeed;
    }
    
    /** Pull cloth **/
    const double force = 12.0;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cloth.addForce(Vec3(0.0, 0.0, -force));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cloth.addForce(Vec3(0.0, 0.0, force));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cloth.addForce(Vec3(-force, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cloth.addForce(Vec3(force, 0.0, 0.0));
    }
}
