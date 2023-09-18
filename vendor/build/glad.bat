gcc -c ..\glad\src\glad.c -I..\glad\include
if not exist "..\lib" mkdir ..\lib
ar -rvs ..\lib\glad.lib *.o
del *.o
