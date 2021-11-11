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

include_directories(
	src
	dependencies/glad/include
	dependencies/glfw/include
	dependencies/stb_image/include
)

add_subdirectory(src)
add_executable(voxel-game-run "src/main.cpp")
target_link_libraries(voxel-game-run voxel-game-lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw)

set(gtest_force_shared_crt True)
add_subdirectory(dependencies/googletest)
add_subdirectory(test)