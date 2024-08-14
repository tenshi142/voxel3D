#include "control.h"

Control::Control(){}

Control::Control(Window win, Axis3D position)
{
    this->win = win;
    this->position = vec3(position.x, position.y, position.z);

    //Initialize values
    //position = vec3(0, 0, 5);
    
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
    initialFov = 45.0f;

    speed = 3.0f;
    mouseSpeed = 0.005f;

    //cout << "Contorl Initialized." << endl;
}

Control::Control(GLFWwindow* window, int width, int height)
{
    //Initialize values
    position = vec3(0, 0, 5);
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
    initialFov = 45.0f;

    speed = 3.0f;
    mouseSpeed = 0.005f;
}

Control::~Control()
{

}

void Control::ComputeMatricesFromInput(double delta)
{
    //Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(win.window, &xpos, &ypos);

    //Reset mouse position for next frame
    glfwSetCursorPos(win.window, win.width/2, win.height/2);

    //Compute new orientation
    horizontalAngle += mouseSpeed * float(win.width/2 - xpos);
    verticalAngle += mouseSpeed * float(win.height/2 - ypos);

    float threhold = 1.6f;
    if (verticalAngle > threhold)
        verticalAngle = threhold;

    if (verticalAngle < -threhold)
        verticalAngle = -threhold;
    
    //Direction: Spherical coordinates to Cartesian coordinates conversion
    vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    //Right vector
    vec3 right = vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    //Up vector
    vec3 up = cross(right, direction);

    //temp speed
    float speed_mod = speed;

    //Sprint Key
    if (glfwGetKey( win.window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
        speed = 30.0f;
    else if (glfwGetKey (win.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        speed = 1.0f;
    else
        speed = 3.0f;

	// Move forward
	if (glfwGetKey( win.window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * (float)delta * speed;
	}
	// Move backward
	if (glfwGetKey( win.window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * (float)delta * speed;
	}
	// Strafe right
	if (glfwGetKey( win.window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * (float)delta * speed;
	}
	// Strafe left
	if (glfwGetKey( win.window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * (float)delta * speed;
	}

    float fov = initialFov; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    //Projection matrix: 45* Fov, 4:3 ratio, display range: 0.1 unit <-> 100 units
    projectionMatrix = perspective(radians(fov), 4.0f/3.0f, 0.1f, 100.0f);
    //Camera matrix
    viewMatrix = lookAt(
        position, //Camera is here
        position + direction, //and looks here: at the same position, plus "direction"
        up //Head is up (set to 0, -1, 0 to look upside-down)
    );
}

vec3 Control::GetControlPosition()
{
    return position;
}

mat4 Control::GetViewMatrix()
{
    return viewMatrix;
}

mat4 Control::GetProjectionMatrix()
{
    return projectionMatrix;
}