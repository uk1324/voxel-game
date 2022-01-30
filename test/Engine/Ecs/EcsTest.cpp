#include <gtest/gtest.h>

#include <Engine/Ecs/EntityManager.hpp>

class EcsTest : public ::testing::Test
{
protected:
	EcsTest()
		: manager(10)
	{}

protected:
	EntityManager manager;
};

TEST_F(EcsTest, AddingComponents)
{
	manager.registerComponent<int>();
	Entity entity = manager.createEntity();
	manager.addComponent<int>(entity, 2);
	EXPECT_EQ(manager.getComponent<int>(entity), 2);
}

TEST_F(EcsTest, RemovingComponents)
{
	manager.registerComponent<int>();
	manager.registerComponent<float>();
	Entity entity = manager.createEntity();
	manager.addComponent<int>(entity, 5);
	manager.addComponent<float>(entity, 1.5f);
	manager.removeComponent<int>(entity);
	manager.update();
	EXPECT_EQ(manager.getComponent<float>(entity), 1.5f);
}

TEST_F(EcsTest, IteratingComponents)
{
	std::vector<int> values = { 1, 2, 3, 4, 5 };
	static constexpr int SUM = 15;

	manager.registerComponent<int>();
	for (int value : values)
	{
		Entity entity = manager.createEntity();
		manager.addComponent<int>(entity, value);
	}

	int sum = 0;
	for (const auto& [entity, component]: manager.getComponents<int>())
	{
		sum += component;
	}
	EXPECT_EQ(sum, SUM);
}

