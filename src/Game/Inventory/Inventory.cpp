#include <Game/Inventory/Inventory.hpp>
#include <Math/Angles.hpp>
#include <Math/Quat.hpp>

static const float squareTrianglesVertices[] = {
	// vertexPos  textureCoord
	-1.0f,  1.0F, 0.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
};

static const float cubeTriangleVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,

	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
};

//static constexpr float squareTriagnelVertexSize;

#include <iostream>

Inventory::Inventory()
	: m_squareVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_cubeTrianglesVbo(cubeTriangleVertices, sizeof(cubeTriangleVertices))
	, m_uiShader("src/Game/Inventory/uiShader.vert", "src/Game/Inventory/uiShader.frag")
	, m_3dItemShader("src/Game/Inventory/3dItem.vert", "src/Game/Inventory/3dItem.frag")
	, m_inventoryCellTexture("assets/textures/ui/inventory_cell.png")
{
	m_squareVao.bind();
	m_squareVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Gfx::VertexArray::setAttribute(1, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 6, false));
	Gfx::VertexArray::setAttribute(1, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, sizeof(float) * 3, sizeof(float) * 6, false));
	Gfx::VertexArray::setAttribute(2, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 1, sizeof(float) * 5, sizeof(float) * 6, false));
}

void Inventory::render(const BlockData& blockData, const InputManager& input, float width, float height)
{
	Vec2 mousePos = input.mousePos();
	mousePos /= Vec2(width, height) * 0.5;
	mousePos -= Vec2(1, 1);
	mousePos.y = -mousePos.y; 

	m_squareVao.bind();
	m_uiShader.use();
	m_inventoryCellTexture.bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	Vec2 size(0.1f / width * height, 0.1f); 
	Vec2 position(0.0f, -1.0f + size.y);
	size.x *= 9;
	Vec2 textureScale(9, 1);
	m_uiShader.setTexture("textureSampler", 0);
	m_uiShader.setVec2("offset", position);
	m_uiShader.setVec2("scale", size);
	m_uiShader.setVec2("textureCoordScale", textureScale);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_BLEND);

	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	blockData.textureArray.bind();
	m_3dItemShader.setTexture("textureArraySampler", 0);
	m_3dItemShader.use();
	Quat rotation = Quat(degToRad(-35.264), Vec3::xAxis) * Quat(degToRad(cos(glfwGetTime()) * 180), Vec3::yAxis);
	//Quat rotation = Quat(degToRad(-35.264), Vec3::xAxis) * Quat(degToRad(-45.0f), Vec3::yAxis);
	Mat4 mat = Mat4::identity;

	mat = mat * Mat4::scale(Vec3(height * 0.05));
	mat = mat * rotation.rotationMatrix();
	mat = mat * Mat4::orthographic(Vec3(0, 0, 0.0f), Vec3(width, height, 1000.0f));
	mat = mat * Mat4::translation(Vec3(position.x, position.y, 0));

	m_3dItemShader.setMat4("projection", mat);

	//m_3dItemShader.setMat4("projection",
	//	rotation.rotationMatrix() 
	//	* Mat4::scale(Vec3(height * 0.05))
	//	* Mat4::translation(Vec3(position.x, position.y, 0))
	//	* Mat4::orthographic(Vec3(0, 0, 0.0f), Vec3(width, height, 1000.0f))
	//);
	m_cubeTrianglesVao.bind();
	// front = 0
	// back = 1
	// bottom = 4
	// top = 5

	m_3dItemShader.setUnsignedInt("faceTextureIndex[0]", blockData[BlockType::Debug].frontTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[1]", blockData[BlockType::Debug].backTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[2]", blockData[BlockType::Debug].rightTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[3]", blockData[BlockType::Debug].leftTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[4]", blockData[BlockType::Debug].bottomTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[5]", blockData[BlockType::Debug].topTextureIndex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
