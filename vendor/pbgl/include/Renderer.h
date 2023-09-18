#pragma once
#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define PBGL_DEVELOPMENT

#if defined(PBGL_DEVELOPMENT)
    #include <iostream>
    #define GLCall(x) GLClearError(); x; if(GLCheckError()) std::cout << "on file" __FILE__ << " Line: " << __LINE__ << std::endl
    #define ASSERT(x) if(!(x)) std::cout<<"Assertion failed in: " __FILE__ " line: " << __LINE__ << " \n"
    #define ASSERTUNIFORM(x) if(!(x)) std::cout<<"Unused uniform \n"
#else 
    #define GLCall(x) x
    #define ASSERT(x) x
    #define ASSERTUNIFORM(x) x
#endif

void GLClearError();
bool GLCheckError();

class Renderer
{
    public:
        void Clear() const;
        void DrawTriangleMesh(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
