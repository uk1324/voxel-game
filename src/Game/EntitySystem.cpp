#include <Game/EntitySystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Rendering/RenderingSystem.hpp>

void EntitySystem::update(EntityManager& entites)
{
	
}

void EntitySystem::spawnZombie(const Vec3& position, EntityManager& entityManager)
{
	Entity entity = entityManager.createEntity();
	entityManager.addComponent(entity, Position{ position });
	entityManager.addComponent(entity, Rotation{ Quat::identity });
	PhysicsAabbCollider collider;
	//collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
	collider.centerOffset = Vec3(0, (1.72 - 0.5 * (1.875)), 0);
	collider.halfSize = Vec3(0.6, 1.875, 0.6) / 2.0f;
	entityManager.addComponent(entity, collider);
	entityManager.addComponent(entity, PhysicsVelocity{});
	entityManager.addComponent(entity, Grounded{});
	entityManager.addComponent(entity, ModelComponent{});
}

Entity EntitySystem::spawnItemEntity(EntityManager& entityManager, const Vec3& position, ItemStack item, const Vec3& velocity)
{
	Entity entity = entityManager.createEntity();
	entityManager.addComponent(entity, Position{ position });
	entityManager.addComponent(entity, ItemComponent{ item, Time::currentTime() });
	entityManager.addComponent(entity, PhysicsVelocity{ velocity });
	entityManager.addComponent(entity, Grounded{});
	entityManager.addComponent(entity, PhysicsAabbCollider{ Vec3(0), Vec3(0.25f) });
	return entity;
}

