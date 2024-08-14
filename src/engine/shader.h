#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Shader {
public:
    // Shader program ID
    GLuint programID;

    Shader();

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use the shader
    void use();

    // Get Program ID
    GLuint getProgramID();

    // Set uniforms
    void setMat4(const string& name, const mat4& value);
    void setVec3(const string& name, const vec3& value);

private:
    // Utility function for checking compilation/linking errors
    void checkCompileErrors(GLuint shader, string type);
};

#endif // SHADER_H
