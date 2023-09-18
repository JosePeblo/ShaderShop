gcc -c ..\glfw\src\*.c -D_GLFW_WIN32
if not exist "..\lib" mkdir ..\lib
ar -rvs ..\lib\glfw3.lib *.o
del *.o


