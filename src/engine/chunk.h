#ifndef CHUNK_H
#define CHUNK_H

#include <algorithm>
#include <cmath>
#include <execution>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include "voxel_mesh.h"

class Chunk
{
    private:
        int height;
        float terrainHeight;
        float chunkSize;
        //Axis2D chunkPosition[];

        vec3 minPoint;
        vec3 maxPoint;

        Range heightLimit;
        Shader shader;

        vector<Axis2D> chunkPosition;
        vector<Axis3D> offset;
        vector<Axis3D> scaling;

    public:
        Chunk(Shader shader, float chunkSize, int height, Range heightLimit);
        ~Chunk();
        float GetTerrainHeight();
        void SetChunkSize(float chunkSize);
        float GetChunkSize();
        void ComputeVoxelGeneration(vector<Axis2D> chunkPosition, int renderType, int spacing);
        void Render(double delta, Window& win, Control& control);
        void GenVoxelChunk(double delta, Window& win, Control& control);
};

#endif