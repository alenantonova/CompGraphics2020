//
// Created by Ilya Gridasov on 03/05/2020.
//

#include "init.hpp"

int initProgram(GLFWwindow*& window) {
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Shooter Game", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024.0 / 2, 768.0 / 2);

    // Dark blue background
    glClearColor(0.02f, 0.1f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    return 0;
}

GLuint initVertexArray() {
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    return VertexArrayID;
}

std::pair<GLuint, GLuint> initTextureShader() {
    // Create and compile our GLSL program from the shaders
    GLuint ID = LoadShaders("shaders/TransformVertexShader.vertexshader", "shaders/TextureFragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(ID, "MVP");
    return {ID, MatrixID};
}

std::pair<GLuint, GLuint> initSimpleShader() {
    GLuint ID = LoadShaders("shaders/TransformVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(ID, "MVP");
    return {ID, MatrixID};
}
