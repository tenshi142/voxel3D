#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(){}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Read the vertex and fragment shader code from file
    string vertexCode, fragmentCode;
    ifstream vertexShaderFile, fragmentShaderFile;

    vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    //std::cout << "Vertex Shader Path:\n" << vertexPath << "\n";
    //std::cout << "Fragment Shader Path:\n" << fragmentPath << "\n";

    try {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();

        // Print shader code for debugging
        //std::cout << "Vertex Shader Code:\n" << vertexCode << "\n";
        //std::cout << "Fragment Shader Code:\n" << fragmentCode << "\n";

    } catch (ifstream::failure& e) {
        cout << "Error: Failed to read shader file. " << e.what() << endl;

        if (vertexShaderFile.fail()) {
            cerr << "Error flag is set (failbit)." << endl;
        }
        if (vertexShaderFile.bad()) {
            cerr << "Error flag is set (badbit)." << endl;
        }
        if (vertexShaderFile.eof()) {
            cerr << "End-of-file reached." << endl;
        }
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile shaders
    GLuint vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    //std::cout << "Program ID: " << programID << std::endl;

    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    //std::cout << "Shader Used" << std::endl;
    glUseProgram(programID);
}

GLuint Shader::getProgramID(){
    return programID;
}

void Shader::setMat4(const string& name, const mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void Shader::setVec3(const string& name, const vec3& value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::checkCompileErrors(GLuint shader, string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            cout << "Error: Shader compilation error of type: " << type << "\n" << infoLog << "\n";
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            cout << "Error: Shader linking error of type: " << type << "\n" << infoLog << "\n";

            // Add the following print statements
            //std::cout << "Vertex Shader Code:\n" << vertexCode << "\n";
            //std::cout << "Fragment Shader Code:\n" << fragmentCode << "\n";
        }
    }

   // std::cout << "Shader Compilation Log:\n" << infoLog << std::endl;
}
