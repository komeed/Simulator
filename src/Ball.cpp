//
// Created by Omeed on 4/30/25.
//

#include "Ball.h"

#include <iostream>

Ball::Ball(glm::vec3 position, float mass, float radius, int subdivisions, std::array<float, 3> color) : Icosphere(radius, subdivisions, true), mass(mass), color(color) {
    pos = position*(SCR_HEIGHT*MPR);
    lastP = pos;
    dPos = glm::vec3(0.0f, 0.0f, 0.0f);
    accel = glm::vec3(0.0f, 0.0f, 0.0f);
    sigmaForce = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Ball::checkBounds() {
    if (useConstraint) {
        float d = glm::length(pos);
        float maxDist = LARGERADIUS - getAdjustedRadius();

        if (d > maxDist) {
            pos = pos*(maxDist)/d;
        }
    }
}

glm::vec2 Ball::returnFinalV(float m1, float m2, float v1, float v2) {
    // Apply the 1D elastic collision formula for each ball's final velocity
    float v1_final = ((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2);
    float v2_final = ((m2 - m1) * v2 + 2 * m1 * v1) / (m1 + m2);

    return glm::vec2(v1_final, v2_final);
}



void Ball::updatePosition(float deltaTime) {
    velocity = pos - lastP;
    lastP = pos;
    pos = pos + velocity + accel * deltaTime * deltaTime;
    checkBounds();
    sigmaForce = glm::vec3(0.0f, 0.0f, 0.0f);
    updateBoxPos();
    //std::cout << roundedPos.x << ", " << roundedPos.y << ", " << roundedPos.z << std::endl;
    //std::cout << RLARGERADIUS << std::endl;
    trans = glm::translate(glm::mat4(1.0f), (pos)/(SCR_HEIGHT*MPR));
}

void Ball::applyForce(glm::vec3 force) {
    sigmaForce += force;
    accel = sigmaForce / mass;
}

void Ball::render() {
    initUniforms();
    drawSphere();
}
void Ball::checkCollision(std::vector<Ball *> balls) {
    for (Ball* ball : balls) {
        if (this != ball) {
            collide(this, ball);
        }
    }
}

void Ball::collide(Ball* ball1, Ball* ball2) {
    float dx = ball1->getAdjustedRadius() + ball2->getAdjustedRadius() - glm::distance(ball1->getPos(), ball2->getPos());
    if (dx > glm::epsilon<float>()) {
        glm::vec3 direction = ball1->getPos() - ball2->getPos();
        glm::vec3 unitDir = glm::normalize(direction);
        ball1->pos = ball1->pos + unitDir * dx/2.0f;
        ball1->checkBounds();
        ball2->pos = ball2->pos - unitDir*dx/2.0f;
        ball2->checkBounds();
    }
}
