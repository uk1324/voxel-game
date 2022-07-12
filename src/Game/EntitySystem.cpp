#include <Game/EntitySystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Rendering/RenderingSystem.hpp>
#include <Model/ModelLoader.hpp>
#include <iostream>

EntitySystem::EntitySystem()
	: m_zombieModel(ModelLoader("assets/models/soldier/soldier.gltf").parse())
{}

void EntitySystem::update(EntityManager& entites, const InputManager& input, const BlockSystem& blocks)
{
	for (auto& [entity, zombie] : entites.getComponents<ZombieComponent>())
	{
		auto& model = entites.getComponent<AnimatedModel>(entity);
		const auto elapsed = Time::currentTime() - zombie.start;
		if (zombie.idle)
		{
			if (elapsed > 2.0f)
			{				
				zombie.start = Time::currentTime();
				const auto newRotation = zombie.oldRotation *
					Quat::fromEuler(0, Vec3::randomInRange(Vec3(-PI<float> / 2, 0, 0), Vec3(PI<float> / 2, 0, 0)).x, 0);
				zombie.oldRotation = zombie.newRotation;
				zombie.newRotation = newRotation;
				zombie.idle = false;
				model.playAnimation(3, AnimatedModelPlayType::PlayLoop, 0.1f);
			}
		}
		else
		{
			if (elapsed > 2.0f)
			{
				zombie.idle = true;
				zombie.start = Time::currentTime();
				model.playAnimation(0, AnimatedModelPlayType::PlayLoop, 0.1f);
			}
			else
			{
				const auto rotationTime = 0.2f;
				auto& rotation = entites.getComponent<Rotation>(entity).value;
				if (elapsed < rotationTime)
				{
					rotation = Quat::slerp(zombie.oldRotation, zombie.newRotation, elapsed / rotationTime);
				}
				// TODO Rotation goes in the oposite direction. Fix this.
				const auto acutalRotation = rotation * Quat::fromEuler(0, PI<float>, 0);
				const auto direction = acutalRotation * Vec3::forward;
				auto& velocity = entites.getComponent<PhysicsVelocity>(entity).value;
				const auto& isGrounded = entites.getComponent<Grounded>(entity).value;
				if (isGrounded)
				{
					velocity += direction * 0.004;
				}
				else
				{
					velocity += direction * 0.001;
				}
				if (isGrounded)
				{
					const auto& position = entites.getComponent<Position>(entity).value;
					const auto blockPos = Vec3I((position + direction - Vec3(0, 0, 0)).applied(floor));
					if (blocks.isSolid(blockPos))
					{
						velocity += Vec3(0, 0.17f, 0);
					}
				}
			}
		}
	}
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
	entityManager.addComponent(entity, ZombieComponent{ Time::currentTime(), true, Quat::identity, Quat::identity });
	auto& animator = entityManager.addComponent(entity, AnimatedModel{m_zombieModel});
	animator.playAnimation(0, AnimatedModelPlayType::PlayLoop, 0);
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

