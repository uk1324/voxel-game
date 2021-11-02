add_library(
	voxel-game-lib 
	
	"Json/JsonValue.cpp"
	"Json/JsonPrinter.cpp"
	"Json/JsonParser.cpp"
	"${PROJECT_SOURCE_DIR}/dependencies/glad/src/glad.c"
   "Engine/Window.hpp" "Engine/Window.cpp" "Json/Json.hpp"   "Engine/Init.hpp" "Engine/Init.cpp" "Utils/FileIo.hpp" "Utils/FileIo.cpp" "Engine/Scene.hpp" "Engine/Scene.cpp" "Engine/Ecs/Entity.cpp" "Engine/Ecs/Entity.hpp" "Engine/Ecs/Component.hpp"  "Engine/Ecs/EntityManager.cpp" "Engine/Ecs/EntityManager.hpp" "Engine/Ecs/ComponentPool.hpp"    "Utils/UninitializedArray.hpp" "Engine/Ecs/Component.cpp" "Log/Log.hpp" "Log/Log.cpp" "Engine/Engine.hpp" "Engine/Engine.cpp" "Game/GameScene.cpp" "Game/GameScene.hpp" "Game/Systems/MeshRenderSystem.hpp" "Game/Systems/MeshRenderSystem.cpp" "Engine/Time.cpp" "Engine/Time.hpp" "Engine/Engine.cpp" "Engine/Input/ButtonAction.hpp" "Engine/Input/InputManager.cpp" "Game/Actions/JumpAction.cpp" "Game/Actions/JumpAction.hpp" "Game/Actions/CameraRotateAction.hpp" "Game/Actions/CameraRotateAction.cpp" "Engine/Input/MouseButton.hpp" "Game/Actions/AttackAction.cpp" "Game/Actions/AttackAction.hpp")


target_link_libraries(voxel-game-lib glfw)
#add_executable(voxel-game-run "main.cpp")
#target_link_libraries(voxel-game-run voxel-game-lib)