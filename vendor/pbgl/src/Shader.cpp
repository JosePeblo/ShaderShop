#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filePath): m_filePath(filePath), m_rendererID(0)
{
    Reload();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));

    for(auto& uniform: m_uniformLocationCache)
    {
        void* data = uniform.second.data;
        UNIFORM_TYPE type = uniform.second.type;
        if(data)
        {
            switch (type)
            {
                case UNIFORM_TYPE::NONE:
                break;
                case UNIFORM_TYPE::INT:
                delete (int*)data;
                break;
                case UNIFORM_TYPE::FLOAT:
                delete (float*)data;
                break;
                case UNIFORM_TYPE::DOUBLE:
                break;
                case UNIFORM_TYPE::VEC2f:
                delete (vec2*)data;
                break;
                case UNIFORM_TYPE::VEC3f:
                delete (vec3*)data;
                break;
                case UNIFORM_TYPE::VEC4f:
                delete (vec4*)data;
                break;
                case UNIFORM_TYPE::MAT2f:
                break;
                case UNIFORM_TYPE::MAT3f:
                delete (mat3*)data;
                break;
                case UNIFORM_TYPE::MAT4f:
                delete (mat4*)data;
                break;

            }
        }        
        
    }
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::Reload() 
{
    if(m_rendererID)
    {
        GLCall(glDeleteProgram(m_rendererID));
    }

    m_uniformLocationCache.clear();
    ShaderProgramSource source = ParseShader(m_filePath);
    m_rendererID = CreateShader(source);
}

void Shader::SetNewPath(const std::string& filePath)
{
    m_filePath = filePath;
    Reload();
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    this->Bind();
    if(m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name].location;
    
    GLCall(GLint location = glGetUniformLocation(m_rendererID, name.c_str()));
    if(location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist! \n";
    m_uniformLocationCache[name].location = location;
    
    return location;
}

// Set uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLint location = GetUniformLocation(name);
    if(location != -1)
    {
        m_uniformLocationCache[name].type = UNIFORM_TYPE::INT;

        void* data = m_uniformLocationCache[name].data;
        if(!data)
            data = new int(value);
        else
            *((int*)data) = value;
        
        m_uniformLocationCache[name].data = data;

        GLCall(glUniform1i(location, value));
    }
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLint location = GetUniformLocation(name);
    if(location != -1)
    {
        m_uniformLocationCache[name].type = UNIFORM_TYPE::FLOAT;

        void* data = m_uniformLocationCache[name].data;
        if(!data)
            data = new float(value);
        else
            *((float*)data) = value;
        
        m_uniformLocationCache[name].data = data;

        GLCall(glUniform1f(location, value));
    }
}

void Shader::SetUniform2f(const std::string& name, const vec2& value)
{
    GLint location = GetUniformLocation(name);
    if(location != -1)
    {
        m_uniformLocationCache[name].type = UNIFORM_TYPE::VEC2f;

        void* data = m_uniformLocationCache[name].data;
        if(!data)
            data = new vec2(value);
        else
            *((vec2*)data) = value;
        
        m_uniformLocationCache[name].data = data;

        GLCall(glUniform2f(location, value.x, value.y));
    }

}

void Shader::SetUniform3f(const std::string& name, const vec3& value)
{
    GLint location = GetUniformLocation(name);

    if(location != -1)
    {
        m_uniformLocationCache[name].type = UNIFORM_TYPE::VEC3f;

        void* data = m_uniformLocationCache[name].data;
        if(!data)
            data = new vec3(value);
        else
            *((vec3*)data) = value;
        
        m_uniformLocationCache[name].data = data;
        GLCall(glUniform3f(location, value.x, value.y, value.z));
    }

}

void Shader::SetUniform4f(const std::string& name, const vec4& value)
{
    GLint location = GetUniformLocation(name);

    if(location != -1)
    {
        m_uniformLocationCache[name].type = UNIFORM_TYPE::VEC4f;

        void* data = m_uniformLocationCache[name].data;
        if(!data)
            data = new vec4(value);
        else
            *((vec4*)data) = value;
        
        m_uniformLocationCache[name].data = data;
        GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
    }

}

// TODO: create allocation for matrix objects
void Shader::SetUniformMat3f(const std::string& name, const mat3& matrix)
{
    GLint location = GetUniformLocation(name);
    // UNIFORM_TYPE::MAT3f
    GLCall(glUniformMatrix3fv(location, 1, GL_TRUE, &matrix.matrix[0][0]));
}

void Shader::SetUniformMat4f(const std::string& name, const mat4& matrix)
{
    GLint location = GetUniformLocation(name);
    // UNIFORM_TYPE::MAT4f
    GLCall(glUniformMatrix4fv(location, 1, GL_TRUE, &matrix.matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
        // VERTEX = 0, 
        // TESSCONTROL = 1,
        // TESSEVAL = 2,
        // GEOMERTRY = 3,
        // FRAGMENT = 4
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
            
    }

    // std::cout << ss[(int)ShaderType::VERTEX].str() << "\n\n";
    // std::cout << ss[(int)ShaderType::FRAGMENT].str() << "\n\n";

    return { ss[(int)ShaderType::VERTEX].str(),
             ss[(int)ShaderType::FRAGMENT].str() };
}

unsigned int Shader::CreateShader(const ShaderProgramSource& source)
{
    unsigned int vs = 0;
    unsigned int fs = 0;
    GLCall(unsigned int program = glCreateProgram());
    if(source.VertexSource.length() > 0)
    {
        GLCall(vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource));
        GLCall(glAttachShader(program, vs));
    }
    if(source.FragmentSource.length() > 0)
    {
        GLCall(fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource));
        GLCall(glAttachShader(program, fs));
    }

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    if(source.VertexSource.length() > 0)
    {
        GLCall(glDeleteShader(vs));
    }
    if(source.FragmentSource.length() > 0)
    {
        GLCall(glDeleteShader(fs));
    }

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    //Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE)
    {
        std::string shadertype;

        if(type == GL_VERTEX_SHADER) shadertype = "vertex";
        if(type == GL_FRAGMENT_SHADER) shadertype = "fragment";

        int lenght;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        char* message = (char*)alloca(lenght * sizeof(char));
        GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
        std::cout<<"Failed to compile "<< shadertype <<" shader"<<std::endl;
        std::cout<<message<<std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;

}

ComputeShader::ComputeShader(const std::string& filepath)
{
    m_filePath = filepath;
    m_rendererID = 0;
    std::ifstream stream(filepath);
    std::stringstream ss;
    std::string line;
    while(getline(stream, line))
    {
        ss << line << '\n';
    }
    std::string source = ss.str();
    const char* src = source.c_str();
    GLCall(unsigned int id = glCreateShader(GL_COMPUTE_SHADER));
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE)
    {
        int lenght;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        char* message = (char*)alloca(lenght * sizeof(char));
        GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
        std::cout<<"Failed to compile compute shader"<<std::endl;
        std::cout<<message<<std::endl;
        GLCall(glDeleteShader(id));
        return;
    }

    GLCall(m_rendererID = glCreateProgram());
    GLCall(glAttachShader(m_rendererID, id));
    GLCall(glLinkProgram(m_rendererID));
    GLCall(glValidateProgram(m_rendererID));
    GLCall(glDeleteShader(id));
}

void ComputeShader::Dispatch(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z) const 
{
    GLCall(glUseProgram(m_rendererID));
    GLCall(glDispatchCompute(groups_x, groups_y, groups_z));    
    // GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
    // GLCall(glMemoryBarrier(GL_ALL_BARRIER_BITS));

}

