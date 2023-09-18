#pragma once
#include "cml.h"
#include <string>
#include <unordered_map>

enum UNIFORM_TYPE
{
    NONE = -1,
    INT,
    FLOAT,
    DOUBLE,
    VEC2f,
    VEC3f,
    VEC4f,
    MAT2f,
    MAT3f,
    MAT4f
};

typedef int GLint;
struct Uniform
{
    GLint location = -1;
    UNIFORM_TYPE type = NONE;
    void* data = nullptr;
};


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
    public:
        Shader(){}
        Shader(const std::string& filePath);
        ~Shader();

        void Bind() const;
        void Unbind() const;
        void Reload();
        void SetNewPath(const std::string& filePath);
        // Set uniforms
        void SetUniform1i(const std::string& name, int value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform2f(const std::string& name, const vec2& value);
        void SetUniform3f(const std::string& name, const vec3& value);
        void SetUniform4f(const std::string& name, const vec4& value);

        void SetUniformMat3f(const std::string& name, const mat3& matrix);
        void SetUniformMat4f(const std::string& name, const mat4& matrix);
        std::string GetPath() const { return m_filePath; }
        std::unordered_map<std::string, Uniform>& GetUniformCache() { return m_uniformLocationCache; }

    private:
        std::string m_filePath;
        unsigned int m_rendererID;
        mutable std::unordered_map<std::string, Uniform> m_uniformLocationCache;

        ShaderProgramSource ParseShader(const std::string& filepath);
        unsigned int CreateShader(const ShaderProgramSource& source);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        GLint GetUniformLocation(const std::string& name) const;
};


