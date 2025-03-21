#include "world.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

World::World(const unsigned seed, const unsigned chunkSize, const unsigned blockSize, const unsigned octaves)
    :seed(seed), chunkSize(chunkSize), blockSize(blockSize), octaves(octaves)
{

}

std::vector<std::vector<float>> World::generateChunk(int chunk_x, int chunk_y) 
{
    // chunk_x and chunk_y are the coordinates of each chunk, ie. starting coordinates are (0, 0)
    int chunkOffset_x = chunk_x * chunkSize;
    int chunkOffset_y = chunk_y * chunkSize;

    std::vector<std::vector<vec2>> grid((chunkSize + 1), std::vector<vec2>(chunkSize + 1));
    std::vector<std::vector<float>> map(blockSize * chunkSize, std::vector<float>(blockSize * chunkSize));

    // Generates a random gradient for each chunk border
    for (int y = 0; y < chunkSize + 1; y++) {
        for (int x = 0; x < chunkSize + 1; x++) {
            grid[x][y] = randomGradient(chunkOffset_x+x, chunkOffset_y+y);
        }
    }

    for (int y = 0; y < blockSize * chunkSize; y++) {
        for (int x = 0; x < blockSize * chunkSize; x++) {

            float amplitude = 1.0f;
            float frequency = 1.0f;

            for (int o = 0; o < octaves; o++){

                // Normalises the point so chunk x starts at x 
                vec2 point_norm = vec2(x, y) / (blockSize * frequency);

                // Finds surrounding chunk borders
                int x0 = (int) point_norm.x;
                int y0 = (int) point_norm.y;
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                // Cubic interpolation horizontally 
                float ix0 = interpolate(
                    (point_norm - vec2(x0, y0)).dot(grid[x0][y0]),
                    (point_norm - vec2(x1, y0)).dot(grid[x1][y0]),
                    point_norm.x - x0
                );
                float ix1 = interpolate(
                    (point_norm - vec2(x0, y1)).dot(grid[x0][y1]),
                    (point_norm - vec2(x1, y1)).dot(grid[x1][y1]),
                    point_norm.x - x0
                );

                // Cubic interpolation vertically
                float value = interpolate(ix0, ix1, point_norm.y - y0);

                map[x][y] += (value * amplitude);

                // Reduces the amplitude + increases the frequency for the next octave
                amplitude /= 2;
                frequency *= 2;
            }

            // Clamps the value between -1 and 1
            if (map[x][y] > 1.0f) map[x][y] = 1.0f;
            else if (map[x][y] < -1.0f) map[x][y] = -1.0f;
        }
    }

    return map;
}

float World::interpolate(float a0, float a1, float w) 
{
    // Cubic interpolation to avoid "block" shapes in the noise
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

vec2 World::randomGradient(int ix, int iy) 
{
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;

    // Bunch of magic with primes to hash to a unique value
    a ^= seed;              
    a *= 3284157443;

    b ^= seed;               
    b ^= a << s | a >> (w - s);
    b *= 1911520717;

    a ^= b << s | b >> (w - s);
    a *= 2048419325;

    // Converts value to range [0, 2*Pi]
    float random = a * (3.14159265 / ~(~0u >> 1)); 

    // Converts the input into a unit vector
    vec2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

void World::drawChunk(std::vector<std::vector<float>> chunk, Shader& shader) 
{
    glEnable(GL_DEPTH_TEST);

    int x_width = chunk.size();
    int z_width = chunk[0].size();

    float vertices[x_width * z_width * 3];
    int indices[(x_width-1) * (z_width-1) * 6];

    int index_pos = 0;

    for (int x = 0; x < x_width; x++) {
        for (int z = 0; z < z_width; z++) {
            
            int pos = 3 * (x * z_width + z);

            vertices[pos] = x;
            vertices[pos+1] = chunk[x][z];
            vertices[pos+2] = z; 

            if (x < x_width - 1 && z < z_width - 1) {
                
                indices[index_pos] = x * z_width + z;
                indices[index_pos+1] = (x+1) * z_width + z;
                indices[index_pos+2] = x * z_width + z + 1;

                indices[index_pos+3] = (x+1) * z_width + z;
                indices[index_pos+4] = (x+1) * z_width + z + 1;
                indices[index_pos+5] = x * z_width + z + 1;

                index_pos += 6;
            }
        }
    }       

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();

    mat4 model = mat4::translate(vec3(0.0f, 0.0f, 0.0f));
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model.m);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}