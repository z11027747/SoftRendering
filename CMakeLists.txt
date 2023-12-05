cmake_minimum_required(VERSION 3.0)

project(SoftRendering)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(softrendering ${SRC_LIST})

if(CMAKE_GENERATOR MATCHES "Visual Studio 17 2022")
	link_directories(${PROJECT_SOURCE_DIR}/lib/lib-vc2022)
	target_link_libraries(softrendering ${PROJECT_SOURCE_DIR}/lib/lib-vc2022/glfw3.lib)
else()
	link_directories(${PROJECT_SOURCE_DIR}/lib-mingw)
	target_link_libraries(softrendering ${PROJECT_SOURCE_DIR}/lib/lib-mingw/glfw3.dll)
endif()