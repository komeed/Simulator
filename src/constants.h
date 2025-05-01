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
#define UNUSED(x) (void)(x)

//window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
//meters per pixel (total meters double dimensions bcs retina display)
const float MPR = 0.01f;
const int numTriangles = 400;
//fps
const double TARGET_FRAME_TIME = 1.0f / 30.0f;
const float RADIUS = 1; // in meters
const float ARADIUS = RADIUS/(SCR_HEIGHT*MPR); // in NPR
//constraint data
const float LARGERADIUS = SCR_WIDTH*MPR; // in meters
const float NUMLINES = 40;

#endif //CONSTANTS_H
