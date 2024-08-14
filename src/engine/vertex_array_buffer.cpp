#include "vertex_array_buffer.h"

VertexArrayBuffer::VertexArrayBuffer(){}

VertexArrayBuffer::~VertexArrayBuffer()
{
    glDeleteVertexArrays(1, &vertex_array);
}

void VertexArrayBuffer::GenVertexArray()
{
    //GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
}

GLuint VertexArrayBuffer::GetVertexArrayID()
{
    return vertex_array;
}

void VertexArrayBuffer::GenVertexBuffer(const GLfloat* data, GLsizei size)
{
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexArrayBuffer::GenVertexBufferStruct(const vector<const void*>& data, GLsizei size)
{
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
}

void VertexArrayBuffer::GenFrameBuffer()
{
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
}

void VertexArrayBuffer::SetVertexBuffer(const GLfloat* data, GLsizei size)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

GLuint VertexArrayBuffer::GetVertexBufferID()
{
    return vertex_buffer;
}

void VertexArrayBuffer::bind()
{
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
}

void VertexArrayBuffer::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}