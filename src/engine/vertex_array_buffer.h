#ifndef VERTEX_GEN_H
#define VERTEX_GEN_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class VertexArrayBuffer 
{
    private:
        GLuint vertex_array;
        GLuint vertex_buffer;
        GLuint frame_buffer;

    public:
        VertexArrayBuffer();
        ~VertexArrayBuffer();

        void GenVertexArray();
        GLuint GetVertexArrayID();

        void GenVertexBuffer(const GLfloat* data, GLsizei size);
        void GenVertexBufferStruct(const vector<const void*>& data, GLsizei size);
        GLuint GetVertexBufferID();

        void SetVertexBuffer(const GLfloat* data, GLsizei size);

        void GenFrameBuffer();

        void bind();
        void unbind();
};

#endif