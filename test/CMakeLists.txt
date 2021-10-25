add_executable(
	voxel-game-test
	
)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(voxel-game-test gtest gtest_main)
target_link_libraries(voxel-game-test voxel-game-lib)

add_test(voxel-game-test voxel-game-test)