add_executable(
	voxel-game-test
 "Json/JsonPrinterTest.cpp" "Math/Vec2Test.cpp" "Math/Vec3Test.cpp" "Math/Mat4Test.cpp")

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(voxel-game-test gtest gtest_main)
target_link_libraries(voxel-game-test voxel-game-lib)

add_test(voxel-game-test voxel-game-test)