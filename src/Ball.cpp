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
    trans = glm::translate(glm::mat4(1.0f), pos/(SCR_HEIGHT*MPR));
}

void Ball::applyForce(glm::vec3 force, float mag) {
    sigmaForce += force * mag;
    accel = sigmaForce / mass;
}

void Ball::render() {
    initUniforms(color.data());
    drawSphere();
}
void Ball::checkCollision(std::vector<Ball *> balls) {
    for (Ball* ball : balls) {
        if (this != ball) {
            //float d1 = glm::distance(this->getPos(), ball->getPos());
            float dx = this->getAdjustedRadius() + ball->getAdjustedRadius() - glm::distance(this->getPos(), ball->getPos());
            if (dx > glm::epsilon<float>()) {
                glm::vec3 direction = this->getPos() - ball->getPos();
                glm::vec3 unitDir = glm::normalize(direction);
                this->pos = this->pos + unitDir * dx/2.0f;
                this->checkBounds();
                ball->pos = ball->pos - unitDir*dx/2.0f;
                ball->checkBounds();
            }
        }
    }
}
