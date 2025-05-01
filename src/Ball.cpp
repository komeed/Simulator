//
// Created by Omeed on 4/30/25.
//

#include "Ball.h"
Ball::Ball(glm::vec3 position, float radius, int subdivisions) : Icosphere(radius, subdivisions, true) {
    mass = 1;
    pos = position*(SCR_HEIGHT*MPR);
    initialP = pos;
    dPos = glm::vec3(0.0f, 0.0f, 0.0f);
    accel = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Ball::checkBounds() {
    float d = glm::distance(pos, glm::vec3(0, 0, 0));
    if (d > LARGERADIUS- RADIUS) {
        pos = pos*(LARGERADIUS- RADIUS)/d;
    }
}

void Ball::nextFrame(float deltaTime) {
    applyForce(glm::vec3(0, -1, 0), 9.8f); //apply gravity force
    /*dPos = (initialV * deltaTime) + (0.5f*accel*float((pow(deltaTime, 2)))); //find next position
    dPos = dPos /(SCR_WIDTH*MPR); //convert to NPC coordinates
    initialV = initialV + (accel * deltaTime);
    pos += dPos;*/
    glm::vec3 prevP = initialP;
    initialP = pos;
    pos = ((2.0f*pos) - prevP + accel*float(pow(deltaTime, 2)));
    checkBounds();
    dPos = (pos - initialP)/(SCR_WIDTH*MPR);
    sigmaForce = glm::vec3(0.0f, 0.0f, 0.0f);
    trans = glm::translate(trans, dPos);
    initUniforms(new float[4]{1, 1, 1, 1});
    drawSphere();
}

void Ball::applyForce(glm::vec3 force, float mag) {
    sigmaForce += force * mag;
    accel = sigmaForce / mass;
}