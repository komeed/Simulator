//
// Created by Omeed on 4/22/25.
//

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

std::string Shader::loadShaderSource(const char* filepath) {
    std::ifstream file;
    std::stringstream buffer;

    file.open(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    buffer << file.rdbuf();  // Read the file into the buffer
    file.close();
    return buffer.str();
}

Shader::Shader(const char* vertFile, const char* fragFile) {
    const std::string vertexCode = loadShaderSource(vertFile);
    const std::string fragmentCode = loadShaderSource(fragFile);
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();
    //compile vertex shaders to opengl
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //compile fragment shaders to opengl
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
unsigned int Shader::returnID() {
    return ID;
}