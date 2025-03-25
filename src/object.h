#ifndef OBJECT_H
#define OBJECT_H

#include "shaders/shader.h"

#include <vector>

class Object
{
    public:
        Object(Shader shader, const char* modelPath);
        ~Object();

        void loadObject(const char* modelPath, std::vector<float> &vertices, std::vector<unsigned int> &indices);
        void drawObject();

        unsigned int VAO;
        Shader shader;
        int numVertices = -1;

    private:
        
};  

#endif