g++ -c ..\imgui\*.cpp ..\imgui\backends\imgui_impl_glfw.cpp ..\imgui\backends\imgui_impl_opengl3.cpp -I..\imgui -I..\glfw\include
if not exist "..\lib" mkdir ..\lib
ar -rvs ..\lib\imgui.lib *.o
del *.o