#pragma once

#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/CubeMap.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

// Cool font atlases
// http://dwarffortresswiki.org/index.php/Tileset_repository
// https://www.gridsagegames.com/rexpaint/resources.html

struct SkyboxData
{
	SkyboxData(
		std::string_view right,
		std::string_view left,
		std::string_view top,
		std::string_view bottom,
		std::string_view front,
		std::string_view back);

	Vao cubeVao;
	Vbo cubeVbo;
	CubeMap texture;
	ShaderProgram shader;
};

class Renderer
{
public:
	Renderer() = delete;

public:
	static void drawSkybox(const Mat4& view, const Mat4& projection, SkyboxData& data);
};