//
// Created by Omeed on 4/28/25.
//

#include "Sphere.h"
#include <cmath>

#include "glm/vec3.hpp"
#include <iostream>
Sphere::Sphere(float radius, int numStacks, int numSectors) : radius(radius), numStacks(numStacks), numSectors(numSectors) {}
void Sphere::draw() {
    float sectorStep = 2 * M_PI / numSectors;
    float stackStep = M_PI / numStacks;
    float sectorAngle, stackAngle;
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(radius);
    int vertCount = 1;
    for (int stack = 1; stack <= numStacks; stack++) {
        stackAngle = M_PI / 2 - stack * stackStep;
        for (int sector = 1; sector <= numSectors; sector++, vertCount++) {
            sectorAngle = sector * sectorStep;
            if (stack == 1) {
                indices.push_back(0);
                indices.push_back(sector);
                if (sector != numSectors) {
                    indices.push_back(sector + 1);
                }
                else {
                    indices.push_back(1); //16 + 8 = 24
                }
            }
            else if (stack == numStacks) {
                indices.push_back(vertCount - numSectors);
                int count = numSectors * (numStacks - 1) + 2;
                indices.push_back(count-1);
                if (sector != numSectors) {
                    indices.push_back(vertCount + 1 - numSectors);
                }
                else {
                    indices.push_back(vertCount + 1 - (2*numSectors));
                }
            }
            else {
                int tl = vertCount - numSectors;
                int tr;
                int br;
                if (sector != numSectors) {
                    tr = vertCount + 1 - numSectors;
                    br = vertCount + 1;
                }
                else {
                    tr = vertCount + 1 - (2*numSectors);
                    br = vertCount + 1 - numSectors;
                }
                int bl = vertCount;
                indices.push_back(tl);
                indices.push_back(bl);
                indices.push_back(tr);
                indices.push_back(tr);
                indices.push_back(bl);
                indices.push_back(br);
            }
            if (stack != numStacks) {
                float x = radius * cosf(sectorAngle) * cosf(stackAngle);
                float y = radius * sinf(sectorAngle) * cosf(stackAngle);
                float z = radius * sinf(stackAngle);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }
    }
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-radius);
    /*int lt, rt, lb, rb;
    for (int stack = 0; stack <= numStacks; stack++) {
        indices.push_back(0);
        indices.push_back(stack);
        if (stack == numStacks) {
            indices.push_back(1);
        }
        for (int sector = 1; sector < numSectors; sector++) {

        }
    }*/
}