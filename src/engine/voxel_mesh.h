#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

#include "shader.h"
#include "control.h"
#include "vertex_array_buffer.h"

#include "struct.h"
#include "vertex_data.h"

using namespace std;
using namespace glm;

class VoxelMesh
{
    private:
		bool toggleFrustum = false;
		bool frustumCamera = false;
		bool occlusionCulling = true;

		int octaveCount = 6;

		GLuint vertexArrayID;
		GLuint vertexBufferID;
		GLuint vertexColorID;
        GLuint programID;
        GLuint matrixID;
		GLuint modelMatrixID;

		Shader objectShader;
        VertexArrayBuffer vertexObject;
		VertexID vertexID;

    public:
		VoxelMesh();
		~VoxelMesh();
		VoxelMesh(Shader objShader);
        VoxelMesh(Shader objShader, const GLfloat* bufferData, const GLfloat* colorData);

		void SetVoxelMeshResources();

		void FaceCulling(Axis3D& voxelPosition, Control& control);

        void GenVoxelMesh(double delta, Window& win, Control& control);
		void GenVoxelMeshOffsets(double delta, Window& win, mat4 projectionMatrix, mat4 viewMatrix, Control& control, Axis3D offsets, Axis3D scaling);

		void Render();

		VertexID GetData();
};

#endif