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

using namespace std;

Camera *camera;
const int WIDTH = 10;
const int HEIGHT = 10;

vector<float> vertices;
vector<unsigned int> allIndices;

float defaultVertices[] = {
    // positions         // UVs
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 0
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 1
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 2
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 3
    // Back face
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 4
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 5
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 6
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f  // 7
};


unsigned int indices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Right face
    1, 5, 6,
    6, 2, 1,
    // Back face
    5, 4, 7,
    7, 6, 5,
    // Left face
    4, 0, 3,
    3, 7, 4,
    // Top face
    3, 2, 6,
    6, 7, 3,
    // Bottom face
    4, 5, 1,
    1, 0, 4
};

void generateTerrain() {
    vertices.clear();
    allIndices.clear();

    unsigned int vertexOffset = 0;

    for (int z = 0; z < HEIGHT; ++z) {
        for (int x = 0; x < WIDTH; ++x) {
            float xpos = x * 1.0f;
            float ypos = 0.0f;
            float zpos = z * 1.0f;

            // Add transformed vertices
            for (int i = 0; i < 8; ++i) {
                float vx = defaultVertices[i * 5 + 0];
                float vy = defaultVertices[i * 5 + 1];
                float vz = defaultVertices[i * 5 + 2];
                float u = defaultVertices[i * 5 + 3];
                float v = defaultVertices[i * 5 + 4];

                vertices.push_back(vx + xpos);
                vertices.push_back(vy + ypos);
                vertices.push_back(vz + zpos);
                vertices.push_back(u);
                vertices.push_back(v);
            }

            for (int i = 0; i < (int) (sizeof(indices) / sizeof(unsigned int)); ++i) {
                allIndices.push_back(indices[i] + vertexOffset);
            }

            vertexOffset += 8;  
        }
    }
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.05f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        camera->cameraPos += cameraSpeed * camera->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->cameraPos -= cameraSpeed * camera->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
}

int main() {
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram;

    camera = new Camera();

    cout << "start" << endl;
    if(!glfwInit()) {
        cout << "Failed to start glfw." << endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    cout << "init" << endl;
    GLFWwindow* window = glfwCreateWindow(600, 600, "Name", NULL, NULL);
    if (!window){
        glfwTerminate();
        cout << "Failed to create window." << endl;
        return -1;
    }
    cout << "Create window" << endl;
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "not initialised." << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    generateTerrain();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), allIndices.data(), GL_STATIC_DRAW);

    shaderProgram = createShader();
    glUseProgram(shaderProgram);


    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/blocks.png", &width, &height, &nrChannels, 0);

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


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);
        camera->project(shaderProgram);

        //stuff
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, allIndices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}