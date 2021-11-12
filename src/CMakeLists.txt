add_library(
	voxel-game-lib 
	
	"Json/JsonValue.cpp"
	"Json/JsonPrinter.cpp"
	"Json/JsonParser.cpp"
	"${PROJECT_SOURCE_DIR}/dependencies/glad/src/glad.c"
   "Engine/Window.hpp" "Engine/Window.cpp" "Json/Json.hpp"   "Engine/Init.hpp" "Engine/Init.cpp" "Utils/FileIo.hpp" "Utils/FileIo.cpp" "Engine/Scene.hpp" "Engine/Scene.cpp" "Engine/Ecs/Entity.cpp" "Engine/Ecs/Entity.hpp" "Engine/Ecs/Component.hpp"  "Engine/Ecs/EntityManager.cpp" "Engine/Ecs/EntityManager.hpp" "Engine/Ecs/ComponentPool.hpp"    "Utils/UninitializedArray.hpp" "Engine/Ecs/Component.cpp" "Log/Log.hpp" "Log/Log.cpp" "Engine/Engine.hpp" "Engine/Engine.cpp" "Game/GameScene.cpp" "Game/GameScene.hpp" "Game/Systems/MeshRenderSystem.hpp" "Game/Systems/MeshRenderSystem.cpp" "Engine/Time.cpp" "Engine/Time.hpp" "Engine/Engine.cpp"  "Engine/Input/InputManager.cpp" "Math/GenericVec2.hpp" "Math/Vec2.hpp" "Math/GenericVec3.hpp" "Math/Vec3.hpp" "Math/GenericQuaternion.hpp" "Math/Quaternion.hpp" "Math/GenericMat4.hpp" "Math/Mat4.hpp" "Math/Angles.hpp" "Engine/Graphics/Mesh.cpp" "Engine/Graphics/Mesh.hpp" "Engine/Graphics/Shader.cpp" "Engine/Graphics/Shader.hpp" "Engine/Graphics/ShaderProgram.hpp" "Engine/Graphics/ShaderProgram.cpp" "Engine/Input/MouseButton.hpp" "Game/Systems/PlayerMovementSystem.hpp" "Game/Systems/PlayerMovementSystem.cpp" "Game/Components/Position.hpp" "Game/Components/Position.cpp" "Game/Components/Rotation.cpp" "Game/Components/Rotation.hpp")


target_link_libraries(voxel-game-lib glfw)
#add_executable(voxel-game-run "main.cpp")
#target_link_libraries(voxel-game-run voxel-game-lib)