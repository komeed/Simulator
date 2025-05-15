//
// Created by Omeed on 4/30/25.
//

#include "Ball.h"

#include <iostream>

Ball::Ball(glm::vec3 position, std::array<float, 3> color) : color(color) {
    mass = 1;
    pos = position*(SCR_HEIGHT*MPR);
    lastP = pos;
    dPos = glm::vec3(0.0f, 0.0f, 0.0f);
    accel = glm::vec3(0.0f, 0.0f, 0.0f);
    sigmaForce = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    trans = glm::mat4(1.0f);
}

void Ball::checkCircleBounds() {
    if (useConstraint) {
        float d = glm::length(pos);
        float maxDist = LARGERADIUS - getAdjustedRadius();

        if (d > maxDist) {
            pos = pos*(maxDist)/d;
        }
    }
}

void Ball::checkRectBounds() {
    if (useConstraint) {
        float maxDist = LARGERADIUS - getAdjustedRadius();
        if (pos.x > maxDist) {
            pos.x = maxDist;
        }
        if (pos.x < -maxDist) {
            pos.x = -maxDist;
        }
        if (pos.z > maxDist) {
            pos.z = maxDist;
        }
        if (pos.z < -maxDist) {
            pos.z = -maxDist;
        }
        if (pos.y < -maxDist) {
            pos.y = -maxDist;
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
   // checkCircleBounds();
    checkCircleBounds();
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
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    drawSphere();
}

void Ball::drawSphere() const
{
    glBindVertexArray(sphereBuffer.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereBuffer.EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

///////////////////////////////////////////////////////////////////////////////
// draw lines only
// the caller must set the line width before call this
///////////////////////////////////////////////////////////////////////////////
void Ball::drawLines() const
{
    glBindVertexArray(lineBuffer.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineBuffer.EBO);
    glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);
}

void Ball::drawPoints() const {
    glBindVertexArray(pointBuffer.VAO);
    glDrawArrays(GL_POINTS, 0, combinedVertices.size()/3);
}

void Ball::setLocations(unsigned int shaderProgram) {
    transformLoc = glGetUniformLocation(shaderProgram, "transform");
    colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
}


///////////////////////////////////////////////////////////////////////////////
// generate buffers
///////////////////////////////////////////////////////////////////////////////

void Ball::generateBuffers(std::vector<float> vertices, std::vector<unsigned int> ind, std::vector<unsigned int> lineInd, Buffers sphere, Buffers point, Buffers line, float radius) {
    combinedVertices = vertices;
    indices = ind;
    lineIndices = lineInd;
    this->radius = radius;
    sphereBuffer = sphere;
    lineBuffer = line;
    pointBuffer = point;
}
