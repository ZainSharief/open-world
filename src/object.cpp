#include "object.h"
#include "maths/maths.h"
#include "vertex.h"
#include "dependencies/stb_image.h"

#include <unordered_map>

Object::Object(Shader shader, const char* modelPath)
    :shader(shader), VAO(0)
{
    unsigned int VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    loadObject(modelPath, vertices, indices);
    numVertices = indices.size();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("models/Tree1/baked.png", &textureWidth, &textureHeight, &nrChannels, 0);
    if (data)
    {
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        std::cout << nrChannels << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, 
            textureWidth, textureHeight, 0, 
            nrChannels == 4 ? GL_RGBA : GL_RGB, 
            GL_UNSIGNED_BYTE, data
        );

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
}

void Object::loadObject(const char* modelPath, std::vector<float> &verticies, std::vector<unsigned int> &indices)
{
    std::ifstream file(modelPath);
    if (!file) {
        std::cerr << "Failed to open OBJ file: " << modelPath << std::endl;
        return;
    }

    std::vector<float> vertexArray;
    std::vector<float> vertexTexCoordArray;
    std::vector<float> vertexNormArray;

    std::unordered_map<vertex, unsigned int> vertexMap;
    int startIndex = 0;

    int shapes = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;


        if (prefix == "mtllib") {
            std::string mtlPath;
            iss >> mtlPath;

            std::filesystem::path objPath = modelPath;
            std::filesystem::path mtlFullPath = objPath.parent_path() / mtlPath;

            // read stuff from mtl file
        }

        else if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertexArray.push_back(x);
            vertexArray.push_back(y);
            vertexArray.push_back(z);
        }

        else if (prefix == "vt") {
            float x, y;
            iss >> x >> y;
            vertexTexCoordArray.push_back(x);
            vertexTexCoordArray.push_back(y);
        }

        else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            vertexNormArray.push_back(x);
            vertexNormArray.push_back(y);
            vertexNormArray.push_back(z);
        }

        else if (prefix == "f") {
            std::vector<int> vertexIndices, textIndices, normIndices;
            std::string faceVertex;

            while (iss >> faceVertex) {
                std::replace(faceVertex.begin(), faceVertex.end(), '/', ' ');
                std::istringstream vertexStream(faceVertex);
                
                int vertIndex, texIndex, normIndex;
                if (vertexStream >> vertIndex >> texIndex >> normIndex) {
                    vertexIndices.push_back(vertIndex);
                    textIndices.push_back(texIndex);
                    normIndices.push_back(normIndex);
                }
            }

            if (vertexIndices.size() != 3) {
                std::cerr << "Error: You were too lazy to implement non-triangle textures lock in." << std::endl;
                return;
            }
            
            for (int i = 0; i < 3; i++) 
            {
                vertex vert = vertex(
                    vertexArray[(vertexIndices[i] - 1) * 3],       vertexArray[(vertexIndices[i] - 1) * 3 + 1],        vertexArray[(vertexIndices[i] - 1) * 3 + 2],
                    vertexTexCoordArray[(textIndices[i] - 1) * 2], vertexTexCoordArray[(textIndices[i] - 1) * 2 + 1],
                    vertexNormArray[(normIndices[i] - 1) * 3],     vertexNormArray[(normIndices[i] - 1) * 3 + 1],      vertexNormArray[(normIndices[i] - 1) * 3 + 2]
                );

                if (vertexMap.find(vert) != vertexMap.end()) {
                    indices.push_back(vertexMap[vert]);
                    continue;
                }

                vertexMap[vert] = startIndex++;

                verticies.push_back(vert.x);
                verticies.push_back(vert.y);
                verticies.push_back(vert.z);

                verticies.push_back(vert.u);
                verticies.push_back(vert.v);

                verticies.push_back(vert.nx);
                verticies.push_back(vert.ny);
                verticies.push_back(vert.nz);

                indices.push_back(vertexMap[vert]);
            }
        }
    }
}

void Object::drawObject()
{
    shader.use();
    shader.setInt("texture1", 0);

    mat4 model = mat4::translate(vec3(1.0f, 0.0f, 0.0f));
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model.m);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}