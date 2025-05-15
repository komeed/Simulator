//
// Created by Omeed on 4/30/25.
//

#ifndef BALL_H
#define BALL_H

#include <Icosphere.h>
#include <vector>
#include <Buffers.h>

class Ball {
    //buffers
    Buffers sphereBuffer;
    Buffers lineBuffer;
    Buffers pointBuffer;

    //vertices
    std::vector<float> combinedVertices;
    std::vector<unsigned int> lineIndices;
    std::vector<unsigned int> indices;

    //buffer locations
    unsigned int transformLoc;
    unsigned int colorLoc;
    unsigned int lightColorLoc;

    //generate buffers
    void generateSphereBuffer();
    void generateLineBuffer();
    void generatePointBuffer();

    glm::mat4 trans;

    float mass;
    float radius;

    //verlet:
    glm::vec3 pos; // adjusted to meters
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

    // draw in VertexArray mode
    void drawSphere() const;
    void drawLines() const;
    void drawPoints() const;

    Ball(glm::vec3 position = glm::vec3(0, 0, 0), std::array<float, 3> color = {BALLCOLOR[0], BALLCOLOR[1], BALLCOLOR[2]});
    void generateBuffers(std::vector<float> vertices, std::vector<unsigned int> ind, std::vector<unsigned int> lineInd, Buffers sphere, Buffers point, Buffers line, float radius);
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
    float getAdjustedRadius() { return radius*SCR_HEIGHT*MPR;}
    float getAdjustedRadius2() { return getAdjustedRadius()*2; }
    void checkCircleBounds();
    void checkRectBounds();

    std::array<float, 3> getColor() { return color; }

    void updateBoxPos() { roundedPos = glm::vec3(std::round(pos.x/getAdjustedRadius2()), std::round(pos.y/getAdjustedRadius2()), std::round(pos.z/getAdjustedRadius2())); }
    glm::vec3 getRoundedPos() { return roundedPos; }

    //setting buffers
    Buffers getLineBuffer() const { return lineBuffer; }
    Buffers getPointBuffer() const { return pointBuffer; }
    Buffers getSphereBuffer() const { return sphereBuffer; }

    bool useConstraint = true;

    void setLocations(unsigned int shaderProgram);
};

struct Cell {
    glm::vec3 pos;
    std::vector<Ball*> balls;
};

#endif //BALL_H
