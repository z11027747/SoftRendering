
rmdir -q -s build
mkdir build
pause

cd build
cmake -G "MinGW Makefiles" ..
pause

mingw32-make
pause

cd ..