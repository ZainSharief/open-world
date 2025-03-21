#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <cmath>

#include "maths/maths.h"
#include "shaders/shader.h"

class World
{
    public:
        World(const unsigned seed, const unsigned chunkSize, const unsigned blockSize, const unsigned octaves);
        std::vector<std::vector<float>> generateChunk(int chunk_x, int chunk_y);
        float interpolate(float a0, float a1, float w);
        vec2 randomGradient(int ix, int iy);
        void drawChunk(std::vector<std::vector<float>> chunk, Shader& shader);

    private:
        unsigned seed;
        unsigned chunkSize;
        unsigned blockSize;
        unsigned octaves;
};  

#endif