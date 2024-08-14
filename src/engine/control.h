#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "struct.h"

using namespace std;
using namespace glm;

class Control
{
    private:
        mat4 viewMatrix;
        mat4 projectionMatrix;

        vec3 position;
        float horizontalAngle;
        float verticalAngle;
        float initialFov;

        float speed;
        float mouseSpeed;

        GLFWwindow* window;
        int width;
        int height;

        Window win;

        double lastTime;

    public:
        Control();
        Control(Window win, Axis3D position);
        Control(GLFWwindow* window, int width, int height);
        ~Control();
        vec3 GetControlPosition();
        mat4 GetViewMatrix();
        mat4 GetProjectionMatrix();
        void ComputeMatricesFromInput(double delta);

};

#endif