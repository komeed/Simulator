//
// Created by Omeed on 5/3/25.
//

#include "Solver.h"

#include <ostream>

Solver::Solver(unsigned int shaderProgram) : shaderProgram(shaderProgram) {
    initCells();
    sigmaForce = glm::vec3(0, 0, 0);
}

void Solver::initCells() {

    for (int x = -RLARGERADIUS; x <= RLARGERADIUS; x++) {
        std::vector<std::vector<Cell>> ycells;
        for (int y = -RLARGERADIUS; y <= RLARGERADIUS; y++) {
            std::vector<Cell> zcells;
            for (int z = -RLARGERADIUS; z <= RLARGERADIUS; z++) {
                zcells.push_back({glm::vec3(x, y, z), {}});
            }
            ycells.push_back(zcells);
        }
        cells.push_back(ycells);
    }
}

void Solver::nextFrame(float dt) {
    applyForce(glm::vec3(0, -1, 0), 9.8f);
    for (Ball* ball : balls) {
        ball->applyForce(sigmaForce);
        ball->updatePosition(dt);
        updateStoredCells(ball);
        //checkCellCollision(ball);
        //ball->render();
    }
    for (Ball* ball : balls) {
        checkCellCollision(ball);
        //ball->checkCollision(balls);
        ball->render();
    }
    for (int i = storedIndices.size()-1; i >= 0; i--) {
        cells[storedIndices[i].x][storedIndices[i].y][storedIndices[i].z].balls.clear();
        storedIndices.pop_back();
    }
    sigmaForce = glm::vec3(0, 0, 0);
}
void Solver::updateStoredCells(Ball *ball) { // store rounded locations into cells 3d matrix
    glm::vec3 foo = ball->getRoundedPos();
    storedIndices.push_back(glm::vec3(foo.x+RLARGERADIUS,foo.y+RLARGERADIUS,foo.z+RLARGERADIUS));
    cells[foo.x+RLARGERADIUS][foo.y+RLARGERADIUS][foo.z+RLARGERADIUS].balls.push_back(ball);
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

void Solver::checkCellCollision(Ball *b) {
    int originX = b->roundedPos.x + RLARGERADIUS;
    int originY = b->roundedPos.y + RLARGERADIUS;
    int originZ = b->roundedPos.z + RLARGERADIUS;
    for (int dx = -1; dx <= 1; dx++) {
        int x = dx+originX;
        if (x < 0 || x >= int(cells.size())) {
            continue;
        }
        for (int dy = -1; dy <= 1; dy++) {
            int y = dy+originY;
            if (y < 0 || y >= int(cells[0].size())) {
                continue;
            }
            for (int dz = -1; dz <= 1; dz++) {
                int z = dz+originZ;
                if (z < 0 || z >= int(cells[0][0].size())) {
                    continue;
                }
                std::vector<Ball*> foo = cells[x][y][z].balls;
                for (size_t i = 0; i < foo.size(); i++) {
                    if (foo[i] == b) {
                        continue;
                    }
                    collide(b, foo[i]);
                }

            }
        }
    }
    /*for (int dx = -1; dx <= 1; dx++) {
        int x = dx+originX;
        if (x < 0 || x >= int(cells.size())) {
            continue;
        }
        for (int dy = -1; dy <= 1; dy++) {
            int y = dy+originY;
            if (y < 0 || y >= int(cells[0].size())) {
                continue;
            }
            for (int dz = -1; dz <= 1; dz++) {
                int z = dz+originZ;
                if (z < 0 || z >= int(cells[0][0].size())) {
                    continue;
                }
                std::vector<Ball*> foo = cells[x][y][z].balls;
                for (size_t i = 0; i < foo.size(); i++) {
                    replaceBall(foo[i], glm::vec3(x,y,z));
                }
            }
        }
    }*/
}

void Solver::replaceBall(Ball* ball, glm::vec3 in) {
    auto i1 = std::find(cells[in.x][in.y][in.z].balls.begin(), cells[in.x][in.y][in.z].balls.end(), ball);
    if (i1 != cells[in.x][in.y][in.z].balls.end()) {
        cells[in.x][in.y][in.z].balls.erase(cells[in.x][in.y][in.z].balls.begin(), i1);
        ball->updateBoxPos();
        updateStoredCells(ball);
    }
    else {
        std::cout << "can't find ball" << std::endl;
    }
}

void Solver::collide(Ball* ball1, Ball* ball2) {
    float dx = ball1->getAdjustedRadius() + ball2->getAdjustedRadius() - glm::distance(ball1->getPos(), ball2->getPos());
    if (dx > glm::epsilon<float>()) {
        glm::vec3 direction = ball1->getPos() - ball2->getPos();
        glm::vec3 unitDir = glm::normalize(direction);
        ball1->setPos(ball1->getPos() + unitDir * dx/2.0f);
        ball1->checkBounds();
        ball2->setPos(ball2->getPos() - unitDir*dx/2.0f);
        ball2->checkBounds();
    }
}

void Solver::applyForce(glm::vec3 force, float mag) {
    sigmaForce += force * mag;
}
