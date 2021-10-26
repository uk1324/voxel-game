add_library(
	voxel-game-lib 
	
	"Json/JsonValue.cpp"
	"Json/JsonPrinter.cpp"
	"Json/JsonParser.cpp"
	"${PROJECT_SOURCE_DIR}/dependencies/glad/src/glad.c"
  )


target_link_libraries(voxel-game-lib glfw)
#add_executable(voxel-game-run "main.cpp")
#target_link_libraries(voxel-game-run voxel-game-lib)