//
// Created by Omeed on 4/22/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <cmath>
#define UNUSED(x) (void)(x)

//window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
//meters per pixel (total meters double dimensions bcs retina display)
const float MPR = 0.01f;
const int numTriangles = 400;
//fps
const double TARGET_FRAME_TIME = 1.0f / 30.0f;
const float RADIUS = 0.4f; // in meters
const float ARADIUS = RADIUS/(SCR_HEIGHT*MPR); // in NPR
//constraint data
const float LARGERADIUS = SCR_WIDTH*MPR; // in meters
//rounded Large Radius (for cells)
const int RLARGERADIUS = std::round(LARGERADIUS/(RADIUS*2));

const float NUMLINES = 40;

//colors
const float BALLCOLOR[3] = {0.5f, 0.5f, 0.5f};
const float LIGHTCOLOR[3] = {1, 1,1};
const float CONSTRAINTCOLOR[3] = {0.7f, 0, 0};

const glm::vec3 LIGHTPOS(0, 1.2f, 2);

#endif //CONSTANTS_H
