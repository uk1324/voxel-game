#include <Engine/Scene.hpp>
#include <Game/Systems/MeshRenderSystem.hpp>


#include <Engine/Graphics/ShaderProgram.hpp>
class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	ShaderProgram shader;
	unsigned int VBO, VAO;


	void update() override;

private:
	MeshRenderSystem meshRenderSystem;
};