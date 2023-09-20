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

    /* ---Compute shader inputs---
        --- BASE INPUTS ---
        -uvec3 - sizes of the work gorups in this compute shader
        gl_NumWorkGroups;

        -uvec3 - the id of the current work group we are in
        gl_WorkGroupID;

        -uvec3 - the ID of the current invocation we are in with respect to the work group we are in
        gl_LocalInvocationID;

        --- SHORTHAND INPUTS ---
        -uvec3 - the ID of the current invocation we are in with respect to the whole compute shader
        // gl_GlobalInvocationID = gl_WorkGroupID * gl_WorkgroupSize + gl_invocationID
        gl_GlobalInvocationID;

        -uint - the index of the current invocation we are in with respect to the work group we are in
        // gl_LoalInvocationIndex = 
            gl_LocalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y +
            gl_LocalInvocationID.y * gl_WorkGroupSize.x +
            gl_LocalInvocationID.z
        gl_LocalInvocationIndex;
    
    */

   unsigned int screenTex;
   GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &screenTex));
   GLCall(glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
   GLCall(glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
   GLCall(glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
   GLCall(glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
   GLCall(glTextureStorage2D(screenTex, 1, GL_RGBA32F, width, height));
   GLCall(glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));

    ComputeShader compute("res/shaders/blur.comp");

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glActiveTexture(GL_TEXTURE0 + 0));
    GLCall(glBindTexture(GL_TEXTURE_2D, screenTex));
    GLCall(glActiveTexture(GL_TEXTURE0 + 1));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
    gui.PushShader(compute);
    compute.SetUniform1i("u_texture", 1);
    compute.SetUniform1f("u_sum", 0.2);
    compute.SetUniform1i("u_kernelSize", 3);

    // compute.Dispatch(640, 640, 1); 

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
        GLCall(glBindTexture(GL_TEXTURE_2D, screenTex));
        // GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
        compute.Bind();
        compute.Dispatch(ceil(width / 8), ceil(height / 4), 1); // WARP optimization nvidia optimization is for 32 work groups
        // compute.Dispatch(width, height, 1);
        // glBindTextureUnit(0, screenTex);
    
        shader.Bind();
        va.Bind();
        ib.Bind();
        renderer.DrawTriangleMesh(va, ib, shader);
        

        window.swapBuffers();
    }

    return 0;
}
