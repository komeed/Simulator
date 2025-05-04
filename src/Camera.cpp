//
// Created by Omeed on 5/1/25.
//

#include "Camera.h"
Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp) :
cameraPos(cameraPos), cameraTarget(cameraTarget), cameraUp(cameraUp) {
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    radius = std::max(cameraPos.x, cameraPos.z);
}
Camera::Camera(float radius, float height) : radius(radius) {
    cameraPos = glm::vec3(0, height, radius);
    cameraTarget = glm::vec3(0, 0, 0);
    cameraUp = glm::vec3(0, 1, 0);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}
glm::mat4 Camera::rotate(float time) {
    float camX = sin(time/2) * radius;
    float camZ = cos(time/2) * radius;
    cameraPos = glm::vec3(camX, cameraPos.y, camZ);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    return view;
}