#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/chunk.h"
#include "engine/control.h"
#include "engine/shader.h"

using namespace std;
using namespace glm;

const int WIDTH = 1024;
const int HEIGHT = 768;

// Global variables
double lastTime = 0.0;
int frameCount = 0;

// Function to calculate and display FPS
void displayFPS(GLFWwindow* window) {
    double currentTime = glfwGetTime();
    frameCount++;

    // If a second has passed, calculate and display FPS
    if (currentTime - lastTime >= 1.0) {
        double fps = static_cast<double>(frameCount) / (currentTime - lastTime);

        // Reset variables for the next second
        lastTime = currentTime;
        frameCount = 0;

        // Display FPS in the console
        std::cout << "FPS: " << fps << std::endl;

        // Display FPS on the window title
        std::string title = "OpenGL Window - FPS: " + std::to_string(fps);
        //glfwSetWindowTitle(window, title.c_str());
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int args, char** argv)
{
    if(!glfwInit())
    {
        cout << "Failed to initialize GLFW. " << endl;
    }

    // Change the working directory to the directory containing the executable
    filesystem::current_path(filesystem::path(argv[0]).remove_filename());

    glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //control version (OpenGL 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Reject Old OpenGL

    //Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Draw Triangle", nullptr, nullptr);

    if(!window)
    {
        cout << "Failed to Open GLFW Window. " << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Make the context of window on current thread
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Ensure the escape key is being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //set bg color
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Light blue sky color //glClearColor(0.0f, 0.2f, 0.0f, 0.0f); //Default color

    // check opengl version
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl; //segmentation fault

    // Query GPU information
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

    cout << "Renderer: " << renderer << endl;
    cout << "Vendor: " << vendor << endl;

    //Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    //Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    //Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    //Create and compile our GLSL program from the shaders
    Shader  shader("shaders/transform_vertex.vs", "shaders/color_fragment.fs");
    //Shader shader2("src/shaders/.vs", "src/shaders/.fs");

    Window win;
    win.window = window;
    win.width = WIDTH;
    win.height = HEIGHT;

    // //Create Camera
    // Control control(win, Axis3D{0, 0, 5});

    //Implement it in chunk generator
    Range heightLimit;
    heightLimit.min = 0;
    heightLimit.max = 255; //default: 32 minecraft: 255
    Chunk chunk(shader, 16.0f, 32, heightLimit); //Default chunk size: 16, height: 128
    int maxChunkBorder = 2; //5

    //Seed the random number generator
    //srand(static_cast<unsigned int>(time(nullptr)));

    vector<Axis2D> chunkPosition;
    // Generate chunk positions dynamically with both positive and negative values
    for (float x = -maxChunkBorder + 1; x < maxChunkBorder; ++x) {
        for (float y = -maxChunkBorder + 1; y < maxChunkBorder; ++y) {
            chunkPosition.push_back(Axis2D{x, y});
        }
    }

    //Face culling
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    chunk.ComputeVoxelGeneration(chunkPosition, 3, 1);//3

    //Create Camera
    Control control(win, Axis3D{0, chunk.GetTerrainHeight() + 2, 0});

    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);

    do
    {
        //Clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glfwGetTime is called only once the first time this function is called
        static double lastTime = glfwGetTime();

        //Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float delta = float(currentTime - lastTime);

        // Manipulate Control
        control.ComputeMatricesFromInput(delta);

        // Generate meshes in each chunks
        chunk.GenVoxelChunk(delta, win, control);

        // Display FPS
        displayFPS(window);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;
    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);

    glfwTerminate();

    return 0;
}