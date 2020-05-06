// Include standard headers
#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common/texture.hpp"
#include "common/controls.hpp"
#include "common/drawing.hpp"
#include "vertex/sphere.h"
#include "vertex/enemy.h"
#include "vertex/floor.h"
#include "init.hpp"

using namespace glm;
GLFWwindow* window;

int main()
{
    initProgram(window);
    GLuint VertexArrayID = initVertexArray();

	auto[FloorID, FloorMatrixID] = initSimpleShader();
    auto[EnemyID, EnemyMatrixID] = initTextureShader();
    auto[SphereID, SphereMatrixID] = initTextureShader();

	// Load the texture
	// Get a handle for our "myTextureSampler" uniform
	GLuint SphereTexture = loadDDS("picture/fire.dds");
	GLuint SphereTextureID = glGetUniformLocation(SphereID, "myTextureSampler");

    GLuint EnemyTexture = loadDDS("picture/stone.dds");
    GLuint EnemyTextureID = glGetUniformLocation(EnemyID, "myTextureSamplerEnemy");

	GLuint floorVertexBuffer = initBuffer(g_vertex_buffer_data_floor);
	GLuint enemyVertexBuffer = initBuffer(g_vertex_buffer_data_enemy);
	GLuint enemyUVBuffer = initBuffer(g_uv_buffer_data_enemy);
    GLuint sphereVertexBuffer = initBuffer(g_vertex_buffer_data_sphere);
    GLuint sphereUVBuffer = initBuffer(g_uv_buffer_data_sphere);

	std::vector<std::vector<float> > enemy_coords;
	int enemy_count = 0;
	const float enemy_radius = 20.0f;

	std::vector<std::vector<glm::vec3> > sphere_coords;
	float sphere_speed = 15.0f;

	const double beginTime = glfwGetTime();

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		//FLOOR
		// Use our shader
		glUseProgram(FloorID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(FloorMatrixID, 1, GL_FALSE, &MVP[0][0]);
		drawFloor(floorVertexBuffer);
    
		//TIME
		double currentTime = glfwGetTime();
		static double lastTime = glfwGetTime();
		auto deltaTime = float(currentTime - lastTime);
		
		//ENEMY
		glUseProgram(EnemyID);

		//SET ENEMY NUMBER AS 12
		if (currentTime - beginTime > enemy_count && enemy_count < 12) {
			std::vector<float> new_enemy = std::vector<float>({
				(float)std::rand() / RAND_MAX * 2 * enemy_radius - enemy_radius + getPosition()[0],
				(float)std::rand() / RAND_MAX * enemy_radius + 2.0f,
				(float)std::rand() / RAND_MAX * 2 * enemy_radius - enemy_radius + getPosition()[2],
				(float)std::rand() / RAND_MAX,
				(float)std::rand() / RAND_MAX,
				(float)std::rand() / RAND_MAX,
				((float)std::rand() / RAND_MAX) * 360.0f,
			});
			enemy_coords.push_back(new_enemy);
			enemy_count++;
		}
	  
		for (auto & enemy_coord : enemy_coords) {
			ModelMatrix = glm::mat4(1.0);
			
			glm::vec3 myRotationAxis(enemy_coord[3], enemy_coord[4], enemy_coord[5]);
			ModelMatrix = glm::rotate(enemy_coord[6] + (float)currentTime, myRotationAxis) * ModelMatrix;
			
			glm::vec3 myTranslationVector(enemy_coord[0], enemy_coord[1], enemy_coord[2]);
			ModelMatrix = glm::translate(glm::mat4(), myTranslationVector) * ModelMatrix;

			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(EnemyMatrixID, 1, GL_FALSE, &MVP[0][0]);

            drawEnemy(EnemyTexture, EnemyTextureID, enemyVertexBuffer, enemyUVBuffer);
		}

		// SPHERE

		//Create sphere if mouse was released
		static int oldState = GLFW_RELEASE;
		int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (newState == GLFW_RELEASE && oldState == GLFW_PRESS) {
			std::vector<glm::vec3> new_sphere = std::vector<glm::vec3>({
				getPosition() + getDirection()*5.0f,
				getDirection()
				});
			sphere_coords.push_back(new_sphere);
		}
		oldState = newState;

		//Draw spheres
		for (auto & sphere_coord : sphere_coords) {
			// Use our shader
			glUseProgram(SphereID);

			ModelMatrix = glm::mat4(0.7f);

			glm::vec3 myTranslationVector(sphere_coord[0]);
			ModelMatrix = glm::translate(glm::mat4(), myTranslationVector) * ModelMatrix;

			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(SphereMatrixID, 1, GL_FALSE, &MVP[0][0]);

            drawSphere(SphereTexture, SphereTextureID, sphereVertexBuffer, sphereUVBuffer);
																							
			//Update sphere coords
			sphere_coord[0] += sphere_coord[1] * sphere_speed * deltaTime;
		}

		//Collider
		for (size_t i = 0; i < enemy_coords.size(); i++) {
			glm::vec3 enemy_pos = glm::vec3(enemy_coords[i][0], enemy_coords[i][1], enemy_coords[i][2]);
			for (size_t j = 0; j < sphere_coords.size(); j++) {
				if (glm::distance(enemy_pos, sphere_coords[j][0]) < 0.7+1.2) {
					enemy_coords.erase(enemy_coords.begin()+i);
					sphere_coords.erase(sphere_coords.begin() + j);
					enemy_count--;
				}
			}
		}

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &floorVertexBuffer);

	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(FloorID);
	//glDeleteProgram(programID);
	//glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
