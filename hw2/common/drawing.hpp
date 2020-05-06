//
// Created by Ilya Gridasov on 03/05/2020.
//

#ifndef TUTORIALS_DRAWING_HPP
#define TUTORIALS_DRAWING_HPP

#include <GL/glew.h>

void drawSphere(GLuint SphereTexture, GLuint SphereTextureID, GLuint sphereVertexBuffer, GLuint sphereUVBuffer);
void drawEnemy(GLuint EnemyTexture, GLuint EnemyTextureID, GLuint enemyVertexBuffer, GLuint enemyUVBuffer);
void drawFloor(GLuint floorVertexBuffer);

#endif //TUTORIALS_DRAWING_HPP
