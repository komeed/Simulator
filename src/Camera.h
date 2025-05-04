//
// Created by Omeed on 5/1/25.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "constants.h"


class Camera {
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;
    glm::mat4 view;
    glm::mat4 projection;
    float radius;
public:
    Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp);
    Camera(float radius, float height);
    glm::mat4 rotate(float time);

    //getters/setters
    void setProjection(glm::mat4 proj) { projection = proj; }
    glm::mat4 getProjection() { return projection; }
    void setPosition(glm::vec3 pos) { cameraPos = pos; }
    glm::vec3 getPosition() { return cameraPos; }
    glm::mat4 getView() { return view; }
};



#endif //CAMERA_H
