#pragma once
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Window.h"
#include "Shader.h"

namespace shl
{

class Serializer
{
    ImGuiIO* m_io;
    std::vector<Shader*> m_shaders;
    public:
    Serializer(Window& window)
    {
        ImGui::CreateContext();
        m_io = &ImGui::GetIO(); (void)*m_io;
        m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
        ImGui_ImplOpenGL3_Init(window.getGlslVersion());

        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    }

    void PushShader(Shader& shader)
    {
        m_shaders.push_back(&shader);
    }

    void OnRender()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float f = 0.0f;
        ImGui::Begin("Hello World");

        for(const auto& shad : m_shaders)
        {
            if(ImGui::CollapsingHeader(shad->GetPath().c_str()))
            {
                for(const auto& uniform : shad->GetUniformCache())
                {
                    switch (uniform.second.type)
                    {
                        case UNIFORM_TYPE::NONE:
                            /* code */
                        break;

                        case UNIFORM_TYPE::INT:
                            if(ImGui::InputInt(uniform.first.c_str(), (int*)uniform.second.data))
                            {
                                puts(uniform.first.c_str());
                            }
                        break;

                        case UNIFORM_TYPE::FLOAT:
                        
                            if(ImGui::SliderFloat(uniform.first.c_str(), (float*)uniform.second.data, -1.0f, 5.0f))
                            {
                                shad->SetUniform1f(uniform.first, *(float*)uniform.second.data);
                            }
                        break;
                    
                    }
                }
            }
            
        }

        if(ImGui::CollapsingHeader("Kuwahara"))
        {
            ImGui::Text("hi");
            ImGui::SliderFloat("Float", &f, 0.0f, 1.0f);
        }

        // ImGui::ShowDemoWindow();
        // static float f = 0.0f;
        // static int counter = 0;

        // ImGui::Begin("Hello World");
        // ImGui::Text("Usefull menu");
        // // ImGui::Checkbox("Another Window", &show_imgui_new_window);
        
        // ImGui::SliderFloat("Float", &f, 0.0f, 1.0f);
        // // ImGui::ColorEdit3("Clear color", (float*)&clear_color);

        // if(ImGui::Button("Button"))
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);
        ImGui::End();


        ImGui::Render();
        // int display_w, display_h;
        // glfwGetFramebufferSize(window.get_window(), &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);

        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    ~Serializer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

};

} 

