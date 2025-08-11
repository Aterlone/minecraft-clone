#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    private:
        unsigned int renderDistance;
        glm::vec3 cameraPos, cameraFront, cameraUp;
        glm::mat4 model, view, projection;

    public:
        Camera();
        
        glm::vec3 getPos();
        void setPos(glm::vec3 cameraPos);

        glm::vec3 getFront();
        void setFront(glm::vec3 cameraFront);

        glm::vec3 getUp();
        void setUp(glm::vec3 cameraUp);

        void project(unsigned int shaderProgram);
};

#endif