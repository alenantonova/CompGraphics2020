// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Homework2 - two intersect triangles with rotation", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    GLuint redTriangleID = LoadShaders( "VertexShader.vertexshader", "RedTriangleShader.fragmentshader" );
    GLuint greenTriangleID = LoadShaders( "VertexShader.vertexshader", "BlackTriangleShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID1 = glGetUniformLocation(redTriangleID, "MVP");
    GLuint MatrixID2 = glGetUniformLocation(greenTriangleID, "MVP");

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Model matrix : an identity matrix
    glm::mat4 Model = glm::mat4(1.0f);

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,

            1.0f,  -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            -0.5f,  0.0f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Init camera position
    double rad = 0.0;
    double pi = 3.14;

    vec3 eye = glm::vec3(4, 3, 3);
    vec3 center = glm::vec3(0, 0, 0);
    vec3 up = glm::vec3(0, 1, 0);

    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        glm::mat4 View = glm::lookAt(eye, center, up);

        // ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Model;

        // Update coordinates
        rad += pi / (2*360); // to move rather slow
        eye = glm::vec3(4 * cos(rad), 1, 4 * sin(rad));

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0, // The attribute we want to configure
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glUseProgram(redTriangleID);
        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glUseProgram(greenTriangleID);
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 6);


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(redTriangleID);
    glDeleteProgram(greenTriangleID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

