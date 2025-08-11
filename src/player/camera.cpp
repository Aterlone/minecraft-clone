#include "camera.hpp"

Camera::Camera() {
    renderDistance = 10;
    cameraPos   = glm::vec3(0.0f, 0.0f,  0.0f);
    cameraFront = glm::vec3(0.0f, -0.5f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
}

void Camera::project(unsigned int shaderProgram) {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
}

glm::vec3 Camera::getPos() {
    return cameraPos;
}
void Camera::setPos(glm::vec3 cameraPos) {
    this->cameraPos = cameraPos;
}

glm::vec3 Camera::getFront() {
    return cameraFront;
}
void Camera::setFront(glm::vec3 cameraFront) {
    this->cameraFront = cameraFront;
}

glm::vec3 Camera::getUp() {
    return cameraUp;
}
void Camera::setUp(glm::vec3 cameraUp) {
    this->cameraUp = cameraUp;
}