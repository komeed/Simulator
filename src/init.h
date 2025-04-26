//
// Created by Omeed on 4/25/25.
//

#ifndef INIT_H
#define INIT_H
#include <vector>
#include <constants.h>
#include <iostream>

struct Buffers {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

void initUI() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1); // Adjust for your window dimensions
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);   // Disable depth testing for 2D rendering
    glDisable(GL_CULL_FACE);    // Disable culling
    glDisable(GL_LIGHTING);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

Buffers initBuffers(std::vector<float> vertices, std::vector<unsigned int> indices = {}) {
    Buffers buffer;
    unsigned int EBO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    if (indices.size() > 0) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);
        buffer.EBO = EBO;
    }
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    buffer.VAO = VAO;
    buffer.VBO = VBO;
    return buffer;
}

#endif //INIT_H
