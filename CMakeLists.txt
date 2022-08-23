cmake_minimum_required(VERSION 3.15)

project(voxel-game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if linking errors change option
# googletest\googletest\CMakeLists.txt


# if(MSVC)
#   add_compile_options(/W4)
# else()
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# could use git submodule to for dependencies.
include_directories(
	src
	dependencies/glad/include
	dependencies/glfw/include
	dependencies/stb_image/include
	dependencies/imgui
	dependencies/voxl/src
)

add_subdirectory(src)
# Working directory can be set by clicking debug > debug and launch settings for <target> and settings "currentDir": "${projectDir}"
add_executable(voxel-game-run "src/main.cpp" "dependencies/stb_image/src/stb_image.c"
"dependencies/imgui/imgui.cpp"
"dependencies/imgui/imgui_draw.cpp"
"dependencies/imgui/imgui_impl_glfw.cpp"
"dependencies/imgui/imgui_tables.cpp"
"dependencies/imgui/imgui_widgets.cpp"
"dependencies/imgui/imgui_impl_opengl3.cpp"
"dependencies/imgui/imgui_impl_opengl3.h"
)

add_subdirectory(dependencies/voxl/src)

target_link_libraries(voxel-game-run voxel-game-lib voxl-lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)

set(gtest_force_shared_crt True)
add_subdirectory(dependencies/googletest)
add_subdirectory(test)