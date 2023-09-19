#include <stdio.h>

#include "Window.h"
#include "Serializer.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h" 
#include "VertexArray.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #define STB_IMAGE_WRITE_IMPLEMENTATION

void validateShader(unsigned int id)
{
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)alloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);
        puts("Failed to compile shader");
        puts(message);
    }
}

int main(void)
{
    auto window = shl::Window(640, 640, "Hello World");
    
    if(!window.ok())
    {
        puts("Failed to initialize window");
        return -1;
    }

    auto gui = shl::Serializer(window);


    float verts[24] = { 
        //Coords    //TexCoords
        1.f, -1.f,  1.f,  0.f,
       -1.f, -1.f,  0.f,  0.f,
       -1.f,  1.f,  0.f,  1.f,
        
        1.f,  1.f,  1.f,  1.f,
        1.f, -1.f,  1.f,  0.f,
       -1.f,  1.f,  0.f,  1.f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    VertexArray va;
    VertexBuffer vb(verts, 24 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/frame.glsl");

    // Texture
    unsigned int textureID;
    int width, height, bpp;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* imgBuffer = stbi_load("res/textures/testTexture.jpg", &width, &height, &bpp, 4);
    if(!imgBuffer)
    {
        puts("No image");
        return -1;
    }
    GLCall(glGenTextures(1, &textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

    // GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));


    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(imgBuffer) stbi_image_free(imgBuffer);

    shader.Bind();
    shader.SetUniform1f("u_gamma", 1.0f);
    shader.SetUniform1f("u_saturation", 1.0f);
    shader.SetUniform1f("u_brightness", 0.0f);
    shader.SetUniform1f("u_contrast", 1.0f);
    shader.SetUniform1i("u_texture", 0);
    shader.SetUniform1i("u_kernelSize", 3);
    gui.PushShader(shader);
    // Texture
    
    

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    while (!window.windowShouldClose())
    {
        // GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        window.pollEvents();
        gui.OnRender();
        
        // glPolygonMode(GL_FRONT, GL_LINE);
        GLCall(glClearColor(0.f, 0.f, 0.f, 0.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //Bind texture
        GLCall(glActiveTexture(GL_TEXTURE0 + 0));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureID));


        //Bind shader
        // GLCall(glUseProgram(program));
        shader.Bind();
        va.Bind();
        ib.Bind();
        renderer.DrawTriangleMesh(va, ib, shader);
        
        window.swapBuffers();
    }

    return 0;
}
