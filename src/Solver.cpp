//
// Created by Omeed on 5/3/25.
//

#include "Solver.h"

Solver::Solver(unsigned int shaderProgram) : shaderProgram(shaderProgram) {}
void Solver::nextFrame(float dt) {
    for (Ball* ball : balls) {
        ball->applyForce(glm::vec3(0, -1, 0), 9.8f * ball->getMass());
        ball->updatePosition(dt);
        ball->checkCollision(balls);
        ball->render();
    }
}
void Solver::append(Ball *ball) {
    balls.push_back(ball);
    ball->setLocations(shaderProgram);
}

void Solver::setConstraint(bool set) {
    for (Ball* ball : balls) {
        ball->useConstraint=set;
    }
}