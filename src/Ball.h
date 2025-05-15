//
// Created by Omeed on 4/30/25.
//

#ifndef BALL_H
#define BALL_H

#include <Icosphere.h>
#include <vector>

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

    std::array<float, 3> color;

    glm::vec2 returnFinalV(float m1, float m2, float v1, float v2);

    void collide(Ball* ball1, Ball* ball2);

public:
    glm::vec3 roundedPos;

    Ball(glm::vec3 position = glm::vec3(0, 0, 0), float mass = 1, float radius = 1, int subdivisions = 2, std::array<float, 3> color = {BALLCOLOR[0], BALLCOLOR[1], BALLCOLOR[2]});
    //~Ball();
    void applyForce(glm::vec3 force);
    void updatePosition(float deltaTime);
    void render();
    void checkCollision(std::vector<Ball*> balls);
    //getters
    glm::vec3 getDPos() { return dPos; }
    glm::vec3 getPos() { return pos; }
    void setPos(glm::vec3 pos) { this->pos = pos; }
    float getMass() { return mass; }

    //adjusted radius (based on meters)
    float getAdjustedRadius() { return getRadius()*SCR_HEIGHT*MPR;}
    float getAdjustedRadius2() { return getAdjustedRadius()*2; }
    void checkBounds();

    std::array<float, 3> getColor() { return color; }

    void updateBoxPos() { roundedPos = glm::vec3(std::round(pos.x/getAdjustedRadius2()), std::round(pos.y/getAdjustedRadius2()), std::round(pos.z/getAdjustedRadius2())); }
    glm::vec3 getRoundedPos() { return roundedPos; }
    bool useConstraint = true;
};

struct Cell {
    glm::vec3 pos;
    std::vector<Ball*> balls;
};

#endif //BALL_H
