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
    yaw = 0;
    pitch = -atan(height/radius);
    cameraPos = glm::vec3(0, height, radius);
    cameraTarget = glm::vec3(0, 0, 0);
    cameraUp = glm::vec3(0, 1, 0);
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

glm::mat4 Camera::getView() {
    //std::cout << yaw << " " << pitch << std::endl;
    cameraPos.x = sin(yaw) * radius;
   // cameraPos.z = cos(yaw) * radius;
    cameraPos.z = cos(pitch) * radius;
    cameraPos.y = cos(yaw) * sin(pitch) * radius;
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    return view;
}

