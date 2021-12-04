#include <gtest/gtest.h>

#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Ecs/Component.hpp>

class MyComponent : public Component
{
public:
	MyComponent(int value)
		: value(value)
	{}

public:
	int value;
};

class EcsTest : public ::testing::Test
{
protected:
	EntityManager manager;
};

TEST_F(EcsTest, AddingComponents)
{
	Entity& entity = manager.addEntity();
	manager.entityAddComponent<MyComponent>(entity, 2);
	MyComponent& component = entity.getComponent<MyComponent>();
	EXPECT_EQ(component.value, 2);
}

