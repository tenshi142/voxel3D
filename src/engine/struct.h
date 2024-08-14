#ifndef STRUCT_H
#define STRUCT_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct Window{
    GLFWwindow* window;
    int width;
    int height;
};

struct ShaderPath{
    const char* vertexPath;
    const char* fragmentPath;
};

struct BufferData{
    vector<const void*> vertex_buffer_data;
    vector<const void*> color_buffer_data; //Either color or textures
};

struct VertexID{
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint vertexColorID;
};

struct Axis2D{
    float x;
    float y;
};

struct Axis3D{
    float x;
    float y;
    float z;
};

struct Position2DDouble{
    double x;
    double y;
};

struct Position3DDouble{
    double x;
    double y;
    double z;
};

struct Voxel{
    int type;
    Axis3D pos;
};

struct Range{
    int min;
    int max;
};

struct OctreeNode {
    vec3 position;
    vec3 size;
    bool isLeaf;
    OctreeNode* children[8]; // Pointers to children nodes (if not a leaf)

    OctreeNode(const glm::vec3& pos, const glm::vec3& sz, bool leaf = true)
        : position(pos), size(sz), isLeaf(leaf) {
        for (int i = 0; i < 8; ++i) {
            children[i] = nullptr;
        }
    }

    ~OctreeNode() {
        for (int i = 0; i < 8; ++i) {
            delete children[i];
        }
    }
};


#endif