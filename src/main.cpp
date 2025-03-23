#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaders/shader.h"
#include "dependencies/stb_image.h"
#include "maths/maths.h"

#include "camera.h"
#include "world.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float deltaTime, Camera& camera);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    framebuffer_size_callback(window, width, height);

    glEnable(GL_DEPTH_TEST);

    Shader shader("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    shader.use();
    mat4 projection = mat4::projection(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection.m);

    Camera camera = Camera(vec3(0.0f, -5.0f, -10.0f), vec3(0.0f, 0.0f, -1.0f), 45.0f, 3.0f, 100.0f);

    World world = World(0, 200, 1, 8);
    std::vector<std::vector<float>> chunk = world.generateChunk(0, 0);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();

        processInput(window, deltaTime, camera);

        glClearColor(0.38, 0.58, 0.98, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        
        mat4 view = camera.getView();
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view.m);

        int objectColorLocation = glGetUniformLocation(shader.ID, "objectColor");
        glUniform3f(objectColorLocation, 0.0f, 1.0f, 0.0f);

        int lightPosLocation = glGetUniformLocation(shader.ID, "lightPos");
        glUniform3f(lightPosLocation, 500.0f, 70.0f, 100.0f);

        int lightColorLocation = glGetUniformLocation(shader.ID, "lightColor");
        glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

        world.drawChunk(chunk, shader);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << err << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 
    }

    shader.deleteShader();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float deltaTime, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.incPosition((camera.getFront() * camera.getSpeed()) * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.incPosition(camera.getFront() * -camera.getSpeed() * deltaTime);  
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.incPosition(camera.getRight() * -camera.getSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.incPosition(camera.getRight() * camera.getSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.incPosition(camera.getUp() * camera.getSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.incPosition(camera.getUp() * -camera.getSpeed() * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Position: " << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << std::endl;
        std::cout << "Front: " << camera.getFront().x << ", " << camera.getFront().y << ", " << camera.getFront().z << std::endl;
        std::cout << "Up: " << camera.getUp().x << ", " << camera.getUp().y << ", " << camera.getUp().z << std::endl;
        std::cout << "Right: " << camera.getRight().x << ", " << camera.getRight().y << ", " << camera.getRight().z << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.rotate(quaternion(camera.getRight(), camera.getDirectionSpeed()*deltaTime));
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera.rotate(quaternion(camera.getRight(), -camera.getDirectionSpeed()*deltaTime));
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.rotate(quaternion(vec3(0, 1, 0), camera.getDirectionSpeed()*deltaTime));
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera.rotate(quaternion(vec3(0, 1, 0), -camera.getDirectionSpeed()*deltaTime));
    }   
}