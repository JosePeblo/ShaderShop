g++ -c ..\pbgl\src\*.cpp -I..\pbgl\include -I..\glad\include -I..\cml
if not exist "..\lib" mkdir ..\lib
ar -rvs ..\lib\pbgl.lib *.o
del *.o
