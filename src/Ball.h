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
    glm::vec3 lastP;
    glm::vec3 velocity;
    glm::vec3 accel;
    std::vector<glm::vec3> forces;
    glm::vec3 sigmaForce;

    glm::vec2 returnFinalV(float m1, float m2, float v1, float v2);


public:
    Ball(glm::vec3 position = glm::vec3(0, 0, 0), float mass = 1, float radius = 1, int subdivisions = 2);
    //~Ball();
    void applyForce(glm::vec3 force, float mag);
    void updatePosition(float deltaTime);
    void render();
    void checkCollision(std::vector<Ball*> balls);
    //getters
    glm::vec3 getDPos() { return dPos; }
    glm::vec3 getPos() { return pos; }
    float getMass() { return mass; }

    //adjusted radius (based on meters)
    float getAdjustedRadius() { return getRadius()*SCR_HEIGHT*MPR;}
    void checkBounds();

    bool useConstraint = true;
};



#endif //BALL_H
