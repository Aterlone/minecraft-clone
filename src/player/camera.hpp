#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
        unsigned int renderDistance;
        glm::vec3 cameraPos, cameraFront, cameraUp;
        glm::mat4 model, view, projection;

        Camera();
        void project(unsigned int shaderProgram);
};

#endif