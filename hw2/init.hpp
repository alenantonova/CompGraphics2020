//
// Created by Ilya Gridasov on 03/05/2020.
//

#ifndef TUTORIALS_INIT_HPP
#define TUTORIALS_INIT_HPP

// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>
#include "common/shader.hpp"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int initProgram(GLFWwindow*& window);
GLuint initVertexArray();
std::pair<GLuint, GLuint> initTextureShader();
std::pair<GLuint, GLuint> initSimpleShader();

template <size_t N>
GLuint& initBuffer(const GLfloat (&data)[N]) {
    auto* buffer = new GLuint;
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    return *buffer;
}

#endif //TUTORIALS_INIT_HPP
