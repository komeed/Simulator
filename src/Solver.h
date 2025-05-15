//
// Created by Omeed on 5/3/25.
//

#ifndef SOLVER_H
#define SOLVER_H

#include <constants.h>
#include <Ball.h>
#include <vector>
#include <iostream>

class Solver {
    //std::vector<Ball*> balls;
    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<std::vector<std::vector<Cell>>> cells;
    std::vector<glm::vec3> storedIndices;
    unsigned int shaderProgram;
    glm::vec3 sigmaForce;

    void initCells();

    void checkCellCollision(Ball* b);

    void collide(Ball* ball1, Ball* ball2);

    void updateStoredCells(Ball* ball);

    void replaceBall(Ball* ball, glm::vec3 in);

public:
    Solver(unsigned int shaderProgram);
    void append(std::unique_ptr<Ball> ball);
    void nextFrame(float dt);
    void setConstraint(bool set);
    int returnCount() { return balls.size(); }
    void applyForce(glm::vec3 force, float mag);
    void clear() { balls.clear(); }
};



#endif //SOLVER_H
