//
// Created by Omeed on 4/21/25.
//

#include "Circle.h"
#include <iostream>
#include <ostream>
//check bounds (bounds with box)
void Circle::checkBounds() {
    float d = glm::distance(pos, glm::vec3(0, 0, pos.z));
    if (d > LARGERADIUS- RADIUS) {
        pos = pos*(LARGERADIUS- RADIUS)/d;
    }
}
//initialize vertex and indeces data
Circle::Circle(float r, glm::vec3 *p, int num, int type) : r(r), p(*p), numTriangles(num) {
    mass = 1;
    r = r/(SCR_WIDTH*MPR);
    int ind = type == 0 ? 3 : 2;
    vertexData.resize((numTriangles+ind-2)*3);
    indecesData.resize(numTriangles*ind);
    for (int i = 0; i < numTriangles; i++) {
        float angle = (2*M_PI* i / numTriangles);
        vertexData[i*3+0] = p->x + r*cos(angle);
        vertexData[i*3+1] = p->y + r*sin(angle);
        vertexData[i*3+2] = 0.0f;
        if (type == 0) {
            indecesData[i*3+0] = numTriangles;
            indecesData[i*3+1] = i;
            if (i==numTriangles-1) {
                indecesData[i*ind+2] = 0;
            }
            else {
                indecesData[i*ind+2] = i+1;
            }
        }
        else {
            indecesData[i*2] = i;
            if (i==numTriangles-1) {
                indecesData[i*2+1] = 0;
            }
            else {
                indecesData[i*2+1] = i+1;
            }
        }
    }
    if (type == 0) {
        vertexData[numTriangles*3+0] = p->x;
        vertexData[numTriangles*3+1] = p->y;
        vertexData[numTriangles*3+2] = 0.0f;
    }
}
//initialize all verlet components
void Circle::initializeVerlet() {
    pos = p*(SCR_HEIGHT*MPR);
    dPos = glm::vec3(0.0f, 0.0f, 0.0f);
    initialV = glm::vec3(0.0f, 0.0f, 0.0f);
    initialP = pos;
    //velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    accel = glm::vec3(0.0f, 0.0f, 0.0f);
}
//processing from next frame
void Circle::nextFrame(float deltaTime) {
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
}
