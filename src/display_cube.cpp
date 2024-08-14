#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>

#include <filesystem>

#include "engine/shader.h"

// Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    void main()
    {
        gl_Position = model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(0.7f, 0.5f, 0.2f, 1.0f);
    }
)";

// Cube vertices and indices
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    0, 4, 7, 7, 3, 0,
    1, 5, 6, 6, 2, 1,
    0, 1, 5, 5, 4, 0,
    2, 3, 7, 7, 6, 2,
};

// GLFW window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int args, char** argv)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    std::filesystem::current_path(std::filesystem::path(argv[0]).remove_filename());

    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Isometric Cube Rotation", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // // Create and compile the vertex shader
    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // glCompileShader(vertexShader);

    // // Check for vertex shader compilation errors
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    //     std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    //     return -1;
    // }

    // // Create and compile the fragment shader
    // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    // glCompileShader(fragmentShader);

    // // Check for fragment shader compilation errors
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    //     std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    //     return -1;
    // }

    // // Create shader program and link shaders
    // unsigned int shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);

    // // Check for shader program linking errors
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    //     glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    //     std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    //     return -1;
    // }

    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    Shader shader("src/shaders/vertex.vs", "src/shaders/fragment.fs");

    // Create VAO, VBO, and EBO for the cube
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and copy vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind EBO and copy index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the rotation matrix
    float angle = 0.0f;

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        //glUseProgram(shaderProgram);
        shader.use();

        GLuint prgID = shader.getProgramID();

        //std::cout << "Curren ID: " << prgID << std::endl;

        // Update rotation matrix
        angle += 0.01f;
        if (angle > 360.0f)
            angle -= 360.0f;

        // Create rotation matrix
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));

        // Pass rotation matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(prgID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}
