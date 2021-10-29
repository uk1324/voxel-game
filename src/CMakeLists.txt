add_library(
	voxel-game-lib 
	
	"Json/JsonValue.cpp"
	"Json/JsonPrinter.cpp"
	"Json/JsonParser.cpp"
	"${PROJECT_SOURCE_DIR}/dependencies/glad/src/glad.c"
   "Engine/Window.hpp" "Engine/Window.cpp" "Json/Json.hpp" "Error/Panic.hpp" "Error/Panic.cpp" "Engine/Init.hpp" "Engine/Init.cpp" "Utils/FileIo.hpp" "Utils/FileIo.cpp" "Engine/Scene.hpp" "Engine/Scene.cpp" "Engine/Ecs/Entity.cpp" "Engine/Ecs/Entity.hpp" "Engine/Ecs/Component.hpp"  "Engine/Ecs/EntityManager.cpp" "Engine/Ecs/EntityManager.hpp" "Engine/Ecs/ComponentPool.hpp"   "Engine/Ecs/System.hpp" "Utils/UninitializedArray.hpp" "Engine/Ecs/Component.cpp")


target_link_libraries(voxel-game-lib glfw)
#add_executable(voxel-game-run "main.cpp")
#target_link_libraries(voxel-game-run voxel-game-lib)