//
// Created by Omeed on 5/3/25.
//

#ifndef SOLVER_H
#define SOLVER_H

#include <constants.h>
#include <Ball.h>

class Solver {
    std::vector<Ball*> balls;
    unsigned int shaderProgram;
public:
    Solver(unsigned int shaderProgram);
    void append(Ball *ball);
    void nextFrame(float dt);
};



#endif //SOLVER_H
