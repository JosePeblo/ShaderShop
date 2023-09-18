#include "Renderer.h"
#include <sstream>

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLCheckError()
{
    bool err = false;
    std::stringstream ss;
    while(GLenum error = glGetError())
    {
        err = true;
        ss<< std::hex<<error;
        std::string res(ss.str());
        std::cout << "[OpenGL Error] (0x" << res << ") ";
        // std::cout << "[OpenGL Error] (0x" << res << ")" << std::endl;
    }
    return err;
}


void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::DrawTriangleMesh(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}