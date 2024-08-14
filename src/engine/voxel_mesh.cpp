#include "voxel_mesh.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

VoxelMesh::VoxelMesh(){}

VoxelMesh::~VoxelMesh()
{
    glDeleteVertexArrays(1, &vertexID.vertexArrayID);
    glDeleteBuffers(1, &vertexID.vertexBufferID);
    glDeleteBuffers(1, &vertexID.vertexColorID);
}

VoxelMesh::VoxelMesh(Shader objShader)
{
    vertexObject.GenVertexArray();
    vertexArrayID = vertexObject.GetVertexArrayID();

    vertexObject.GenVertexBuffer(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
    //vertexObject.GenVertexBuffer(bufferData, sizeof(GLfloat) * (sizeof(bufferData)/sizeof(GLfloat)));
    vertexBufferID = vertexObject.GetVertexBufferID();

    vertexObject.GenVertexBuffer(g_color_buffer_data, sizeof(g_color_buffer_data));
    //vertexObject.GenVertexBuffer(bufferData, sizeof(GLfloat) * (sizeof(bufferData)/sizeof(GLfloat)));
    vertexColorID = vertexObject.GetVertexBufferID();

    //cout << vertex << endl;

    this->objectShader = objShader; //Shader("src/shaders/transform_vertex.glsl", "color_fragment.glsl");
    //programID = objectShader.getProgramID();
    //objectShader.use();
    matrixID = glGetUniformLocation(objectShader.getProgramID(), "MVP");

    modelMatrixID = glGetUniformLocation(objectShader.getProgramID(), "modelMatrix"); // Assuming "modelMatrix" is the name of your model matrix uniform

    //cout << "program ID: " << objectShader.getProgramID() << endl;
    objectShader.use();
}

VoxelMesh::VoxelMesh(Shader objShader, const GLfloat* bufferData, const GLfloat* colorData) //Unable to retrieve data for bufferData
{
    vertexObject.GenVertexArray();
    vertexArrayID = vertexObject.GetVertexArrayID();

    vertexObject.GenVertexBuffer(bufferData, sizeof(bufferData));//problem with input data
    //vertexObject.GenVertexBuffer(bufferData, sizeof(GLfloat) * (sizeof(bufferData)/sizeof(GLfloat)));
    vertexBufferID = vertexObject.GetVertexBufferID();

    vertexObject.GenVertexBuffer(colorData, sizeof(colorData));
    //vertexObject.GenVertexBuffer(bufferData, sizeof(GLfloat) * (sizeof(bufferData)/sizeof(GLfloat)));
    vertexColorID = vertexObject.GetVertexBufferID();

    //cout << vertex << endl;

    this->objectShader = objShader; //Shader("src/shaders/transform_vertex.glsl", "color_fragment.glsl");
    //programID = objectShader.getProgramID();
    //objectShader.use();
    matrixID = glGetUniformLocation(objectShader.getProgramID(), "MVP");
    //cout << "MatrixID: " << matrixID << endl;
    objectShader.use();
}

void VoxelMesh::SetVoxelMeshResources()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
    //1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    //Update the buffer data with the modified vertex data
    //glBufferData(GL_ARRAY_BUFFER, tempVertexBufferData.size() * sizeof(GLfloat), tempVertexBufferData.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorID);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
}

void VoxelMesh::GenVoxelMesh(double delta, Window& win, Control& control)
{
    mat4 projectionMatrix = control.GetProjectionMatrix();
    mat4 viewMatrix = control.GetViewMatrix();

    mat4 modelMatrix = mat4(1.0);
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    //1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorID);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle or print the OpenGL error
        cout << "Error Here! " << error << endl;
    }
}

void VoxelMesh::FaceCulling(Axis3D& voxelPosition, Control& control) {
    // Get the camera position from the control object
    vec3 cameraPosition = control.GetControlPosition();

    // Calculate the vector from the camera to the voxel
    vec3 toVoxel = vec3(voxelPosition.x - cameraPosition.x, voxelPosition.y - cameraPosition.y, voxelPosition.z - cameraPosition.z);

    // Get the view matrix from the control object
    mat4 viewMatrix = control.GetViewMatrix();

    // Transform the toVoxel vector into view space
    vec4 toVoxelViewSpace = viewMatrix * vec4(toVoxel, 1.0);

    // Check if the voxel is behind the camera (in view space)
    if (toVoxelViewSpace.z > 0.0) {
        // The voxel is in front of the camera, so render it
        glCullFace(GL_FRONT);
    } else {
        // The voxel is behind the camera, so cull it
        glCullFace(GL_FRONT);
        glCullFace(GL_BACK);
    }
}

//This method only for cube (not suite to other geometry)
void VoxelMesh::GenVoxelMeshOffsets(double delta, Window& win, mat4 projectionMatrix, mat4 viewMatrix, Control& control, Axis3D offsets, Axis3D scaling)
{
    //mat4 modelMatrix = mat4(1.0);
    //mat4 modelMatrix = translate(mat4(1.0), vec3(offsets.x, offsets.y, offsets.z)); //Add scale //step 1. culling step 2. greedy mesh
    mat4 translationMatrix = translate(mat4(1.0), vec3(offsets.x, offsets.y, offsets.z));
    mat4 scaleMatrix = scale(mat4(1.0), vec3(scaling.x, scaling.y, scaling.z));

    //mat4 modelMatrix = translationMatrix * scaleMatrix;
    mat4 mvp = projectionMatrix * viewMatrix * translationMatrix * scaleMatrix; // * modelMatrix;

    // GLenum error = glGetError();
    // if (error != GL_NO_ERROR) {
    //     // Handle or print the OpenGL error
    //     cout << "Error Here! " << error << endl;
    // }

    // // Frustum culling: Check if the voxel block is inside the camera's view frustum
    // if (!IsInFrustum(mvp, minPoint, maxPoint)) {
    //     return;  // Skip rendering if not in the frustum
    // }

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    //1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    //Update the buffer data with the modified vertex data
    //glBufferData(GL_ARRAY_BUFFER, tempVertexBufferData.size() * sizeof(GLfloat), tempVertexBufferData.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexColorID);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    //Enable face culling
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    // //Perform face culling based on visibility
    //FaceCulling(offsets, control);

    glEnable(GL_DEPTH_TEST);

    //glDrawArrays(GL_TRIANGLES, 0, 36); // 12*3 indices starting at 0 -> 12 triangles
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10);

    glDisable(GL_DEPTH_TEST);

    //Disable face culling after rendering
    //glDisable(GL_CULL_FACE);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
}

void VoxelMesh::Render()
{

}

VertexID VoxelMesh::GetData()
{
    return vertexID;
}