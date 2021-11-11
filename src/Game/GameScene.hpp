#include <Engine/Scene.hpp>
#include <Game/Systems/MeshRenderSystem.hpp>


#include <Engine/Graphics/ShaderProgram.hpp>
class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	ShaderProgram* shader;

	void update() override;

private:
	MeshRenderSystem meshRenderSystem;
};