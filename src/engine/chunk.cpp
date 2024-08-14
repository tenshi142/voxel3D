#include "chunk.h"

//All Functions
float genSimpleLandscape(int x, int z, float frequency, float amplitude, float offset) //Default frequency: 0.1f, amplitude: 5.0f, offset: based on height
{
    return amplitude * glm::perlin(glm::vec2(frequency * x, frequency * z)) + offset;
}

OctreeNode* ConvertPositionToOctreeNode(const Axis3D& position)
{
    OctreeNode* octreeNode;
    octreeNode->position = glm::vec3(position.x, position.y, position.z); //segmentation fault
    //octreeNode->size = glm::vec3(10.0f, 10.0f, 10.0f); // Assuming uniform-sized nodes

    // Set other properties of the octree node as needed
    octreeNode->isLeaf = true; // For simplicity, assuming all nodes are initially leaves

    // Initialize children to nullptr
    for (int i = 0; i < 8; ++i) {
        octreeNode->children[i] = nullptr;
    }

    return octreeNode;
}

//class members
Chunk::Chunk(Shader shader, float chunkSize, int height, Range heightLimit)
{
    this->shader = shader;
    this->chunkSize = chunkSize;
    this->height = height;
    this->heightLimit = heightLimit;
    this->terrainHeight = 0;
}

Chunk::~Chunk()
{

}

float Chunk::GetTerrainHeight()
{
    return terrainHeight;
}

void Chunk::SetChunkSize(float chunkSize)
{
    this->chunkSize = chunkSize;
}

float Chunk::GetChunkSize()
{
    return chunkSize;
}

void Chunk::ComputeVoxelGeneration(vector<Axis2D> chunkPosition, int renderType, int spacing)
{
    //Axis3D offsets[chunkSize+height];
    //vector<Axis3D> offset;

    //Clear the offset vector before generating new voxel blocks
    offset.clear();

    for (int i = 0; i < chunkPosition.size(); i++)
    {
        chunkPosition[i].x = chunkPosition[i].x * chunkSize;
        chunkPosition[i].y = chunkPosition[i].y * chunkSize;
    }

    int y_simple_landscape;

    for (int i = 0; i < chunkPosition.size(); i++)
    {
        switch (renderType)
        {
        case 0: //fill all voxel mesh in single chunk
            for(int x = 0; x < chunkSize; ++x)
            {
                for(int y = 0; y < height; ++y)
                {
                    for(int z = 0;z < chunkSize; ++z)
                    {
                        offset.push_back(Axis3D{x + chunkPosition[i].x * spacing, (float) y * spacing, z + chunkPosition[i].y * spacing});
                    }
                }
            }
            terrainHeight = height;
            break;

        case 1: //generate sphere
            for(int x = 0; x < chunkSize; ++x)
            {
                for(int y = 0; y < height; ++y)
                {
                    for(int z = 0;z < chunkSize; ++z)
                    {
                        if(sqrt((float)(x-chunkSize/2)*(x-chunkSize/2)+(y-chunkSize/2)*(y-chunkSize/2)+(z-chunkSize/2)*(z-chunkSize/2))<=chunkSize/2)
                        {
                            offset.push_back(Axis3D{x + chunkPosition[i].x * spacing, (float) y * spacing, z + chunkPosition[i].y * spacing});
                        }
                    }
                }
            }
            terrainHeight = height;
            break;

        case 2: //Generate landscape with perlin noise
            for(int x = 0; x < chunkSize; ++x)
            {
                for(int z = 0;z < chunkSize; ++z)
                {
                    y_simple_landscape = static_cast<int>(genSimpleLandscape(x + chunkPosition[i].x * spacing, z + chunkPosition[i].y * spacing, 0.1, 5, height));

                    offset.push_back(Axis3D{x + chunkPosition[i].x * spacing, (float) y_simple_landscape * spacing, z + chunkPosition[i].y * spacing});
                    
                    if(y_simple_landscape > terrainHeight)
                    {
                        terrainHeight = y_simple_landscape;
                    }
                }
            }
            break;

        case 3: //Generate landscape with perlin noise and fill up the blocks
            for (int x = 0; x < chunkSize; ++x) {
                for (int z = 0; z < chunkSize; ++z) {
                    y_simple_landscape = static_cast<int>(genSimpleLandscape(x + chunkPosition[i].x * spacing, z + chunkPosition[i].y * spacing, 0.1, 5, height));

                    // Fill up to the wave
                    for (int y = 0; y <= y_simple_landscape; ++y) {
                        if(y * spacing >= heightLimit.min && y * spacing <= heightLimit.max)
                        {
                            offset.push_back(Axis3D{x + chunkPosition[i].x * spacing, (float) y * spacing, z + chunkPosition[i].y * spacing});
                        }

                       // Calculate minPoint and maxPoint as new points are added
                        // if(y == 0 && offset.size() == 1) {
                        //     // minPoint = maxPoint = vec3(offset.back().x, offset.back().y, offset.back().z);
                        //     minPoint = vec3(offset.back().x, offset.back().y, offset.back().z);
                        //     maxPoint = vec3(offset.back().x, offset.back().y, offset.back().z);
                        // } else {
                        //     minPoint = glm::min(minPoint, vec3(offset.back().x, offset.back().y, offset.back().z));
                        //     maxPoint = glm::max(maxPoint, vec3(offset.back().x, offset.back().y, offset.back().z));
                        // }

                        //Limit maximum height generated
                        if(y > terrainHeight)
                        {
                            terrainHeight = y;
                        }
                        //Limit minimum height generated

                    }
                }
            }
            break;
        case 4: //New method for rendering
            // for (int x = 0; x < chunkSize; ++x)
            // {
            //     for (int y = 0; y <)
            // }
            break;
        case 5: //Greedy Meshing //face culling
            break;
        case 6: //Octree method
            break;
        default:
            break;
        }
    }
}

void Chunk::Render(double delta, Window& win, Control& control)
{

}

void Chunk::GenVoxelChunk(double delta, Window& win, Control& control) 
{
    VoxelMesh mesh(shader);

    mat4 projectionMatrix = control.GetProjectionMatrix();
    mat4 viewMatrix = control.GetViewMatrix();

    Axis3D scaling;
    scaling.x = 1;
    scaling.y = 1;
    scaling.z = 1;

    for(int i = 0; i < offset.size(); i++)
    {
        mesh.GenVoxelMeshOffsets(delta, win, projectionMatrix, viewMatrix, control, offset[i], scaling); //Lag here //need to use greedy meshing
    }

    // Use parallel execution policy for parallelization
    // for_each(execution::par, offset.begin(), offset.end(), [&](const Axis3D& pos) {
    //     mesh.GenVoxelMeshOffsets(delta, win, projectionMatrix, viewMatrix, pos, scaling);
    // });
}
