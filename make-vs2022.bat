
IF NOT EXIST build (mkdir build)

del /s /q "build\*.*"

cd build
cmake -G "Visual Studio 17 2022" ..

::set msbuild = "D:\S\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
::%msbuild% SoftRendering.sln

cd ..

copy "res\ColoredNumberAtlas.png" "build\"

pause