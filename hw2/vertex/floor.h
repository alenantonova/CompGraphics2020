//
// Created by Ilya Gridasov on 02/05/2020.
//

#ifndef TUTORIALS_FLOOR_H
#define TUTORIALS_FLOOR_H

#include <GL/glew.h>

//floor
static const GLfloat g_vertex_buffer_data_floor[] = {
        -100.0f, 0.0f,-100.0f,
        -100.0f, 0.0f, 100.0f,
        100.0f, 0.0f, 100.0f,

        100.0f, 0.0f, 100.0f,
        100.0f, 0.0f,-100.0f,
        -100.0f, 0.0f,-100.0f,
};

#endif //TUTORIALS_FLOOR_H
