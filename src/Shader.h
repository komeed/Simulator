//
// Created by Omeed on 4/22/25.
//

#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader {
    unsigned int ID;
    std::string loadShaderSource(const char* filepath = 0);
public:
    Shader(const char* vertFile = 0, const char* fragFile = 0);
    unsigned int returnID();
};



#endif //SHADER_H
