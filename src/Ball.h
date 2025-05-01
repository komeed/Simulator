//
// Created by Omeed on 4/30/25.
//

#ifndef BALL_H
#define BALL_H

#include <Icosphere.h>

class Ball : public Icosphere {
    float mass;

    //verlet:
    glm::vec3 pos;
    glm::vec3 dPos;
    glm::vec3 initialP;
    glm::vec3 accel;
    std::vector<glm::vec3> forces;
    glm::vec3 sigmaForce;

    void checkBounds();

public:
    Ball(glm::vec3 position = glm::vec3(0, 0, 0), float radius = 1, int subdivisions = 2);
    //~Ball();
    void nextFrame(float deltaTime);
    void applyForce(glm::vec3 force, float mag);
    //getters
    glm::vec3 getDPos() { return dPos; }

};



#endif //BALL_H
