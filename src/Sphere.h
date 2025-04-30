//
// Created by Omeed on 4/28/25.
//

#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

class Sphere {
    float radius;
    float numStacks;
    float numSectors;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
public:
    Sphere(float radius, int numStacks, int numSectors);
    void draw();
    std::vector<float> getVertices() { return vertices; }
    std::vector<unsigned int> getIndices() { return indices; }
};



#endif //SPHERE_H
