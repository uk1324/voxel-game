#include <Engine/Scene.hpp>
#include <Game/Systems/MeshRenderSystem.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);
	void update() override;

private:
	MeshRenderSystem meshRenderSystem;
};