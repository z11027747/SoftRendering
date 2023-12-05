
IF NOT EXIST build (mkdir build)

del /s /q "build\*.*"

cd build
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

mingw32-make

cd ..

copy "res\ColoredNumberAtlas.png" "build\"
copy "lib\lib-mingw\glfw3.dll" "build\"

pause