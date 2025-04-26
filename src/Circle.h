//
// Created by Omeed on 4/21/25.
//

#ifndef CIRCLE_H
#define CIRCLE_H
#include <vector>
#include <constants.h>
#include <cmath>

class Circle {
    float mass;
    float r;
    glm::vec3 p;
    int numTriangles;
    std::vector<float> vertexData;
    std::vector<unsigned int> indecesData;

    void checkBounds();
    void updateAccel();
public:
    glm::vec3 pos;
    glm::vec3 dPos;
    glm::vec3 initialP;
    glm::vec3 initialV;
    glm::vec3 accel;
    std::vector<glm::vec3> forces;
    glm::vec3 sigmaForce;
    Circle(float r = 0, glm::vec3 *p = 0, int num = 0, int type = 0);
    std::vector<float> retrieveVertexData() {
        return vertexData;
    }
    std::vector<unsigned int> retrieveIndecesData() {
        return indecesData;
    }
    float retrieveR() {
        return r;
    }
    void initializeVerlet();
    void nextFrame(float deltaTime);
    void applyForce(glm::vec3 force, float mag) {
        sigmaForce += force * mag;
        accel = sigmaForce / mass;
    }
};

#endif //CIRCLE_H
