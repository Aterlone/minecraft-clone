#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "player/camera.hpp"
#include "shaders/shader.hpp"
#include "game/world.hpp"

using namespace std;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

Camera *camera;

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.05f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        camera->setPos(camera->getPos() + cameraSpeed * camera->getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->setPos(camera->getPos() - cameraSpeed * camera->getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->setPos(camera->getPos() - glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->setPos(camera->getPos() + glm::normalize(glm::cross(camera->getFront(), camera->getUp())) * cameraSpeed);
}

void createCamera() {
    camera = new Camera();
    std::cout << camera->getPos()[0] << std::endl;
}

void glfwSetup() {
    if(!glfwInit()) {
        cout << "Failed to start glfw." << endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

World setupWorld() {
    World world = World();
    world.checkAndLoadChunks(camera->getPos());
    createCamera();
    glfwSetup();  
    return world;
}

GLFWwindow* setupWindow() {
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Name", NULL, NULL);
    if (!window){
        glfwTerminate();
        cout << "Failed to create window." << endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Not initialised." << endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    return window;
}

vector<unsigned int> generateGPUBuffers(World world) {
    unsigned int VAO, VBO, EBO;
    vector<float> vertices = world.getVertices();
    vector<unsigned int> indices = world.getIndices();

    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    return {VAO, VBO, EBO};
}

unsigned int setupTextures() {
    unsigned int texture;
    // Function Params for loading image
    int width, height, nrChannels;
    // Data from Function
    unsigned char *data;
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("textures/blocks.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB; // fallback

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture at path: ../textures/blocks.png" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

int main() {
    World world;
    GLFWwindow* window;

    unsigned int VAO, EBO; //VBO temporarily deleted.
    unsigned int shaderProgram;
    unsigned int texture;

    vector<float> vertices;
    vector<unsigned int> indices;

    world = setupWorld();
    window = setupWindow();

    vector<unsigned int> bufferValues = generateGPUBuffers(world);
    VAO = bufferValues[0]; 
    // VBO = bufferValues[1]; 
    EBO = bufferValues[2];

    shaderProgram = createShader();
    glUseProgram(shaderProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    texture = setupTextures();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);
        // Change this to chekc based on the chunk it's in not position.
        if (world.checkAndLoadChunks(camera->getPos())) {
            vertices = world.getVertices();
            indices = world.getIndices();
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
            std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << std::endl;

        }

        camera->project(shaderProgram);

        //stuff
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}