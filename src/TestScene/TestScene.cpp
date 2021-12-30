#include <TestScene/TestScene.hpp>
#include <Engine/Engine.hpp>
#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

#include <iostream>

struct Acceleration
{
    Vec3 value;
};

TestScene::TestScene(Engine& engine)
    : Scene(engine, 100)
    , cameraSystem(90.0f, 0.1f, 1000.0f)
    , playerMovementSystem(*this)
{
    registerInputActions();
    registerComponents();
    createPlayer();
    setupGraphics();

    auto random = []()
    {
        return (((rand() % 1000) / 1000.0f));
    };

    for (int i = 0; i < 10; i++)
    {
        Entity entity = entityManager.createEntity();
        Vec3 position(random() * 15, random() * 15, random() * 15);
        entityManager.entityEmplaceComponent<Position>(entity, position);
        Quat rotation(random(), Vec3(random(), random(), random()).normalized());
        entityManager.entityEmplaceComponent<Rotation>(entity, rotation);
        entityManager.entityEmplaceComponent<RenderingComponent>(entity);
    }

    entity = entityManager.createEntity();
    entityManager.entityEmplaceComponent<Position>(entity, Vec3());
    entityManager.entityEmplaceComponent<Rotation>(entity, Quat::identity);
    entityManager.entityEmplaceComponent<RenderingComponent>(entity);
}

void TestScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    entityManager.entityGetComponent<Position>(entity) = entityManager.entityGetComponent<Position>(player).value + Vec3(0, 0, 5);
    entityManager.entityGetComponent<Rotation>(entity) = entityManager.entityGetComponent<Rotation>(player);

    if (input.isButtonDown("attack"))
    {
        Entity e = entityManager.createEntity();
        entityManager.entityEmplaceComponent<RenderingComponent>(e);
        entityManager.entityEmplaceComponent<Position>(e, entityManager.entityGetComponent<Position>(player));
        entityManager.entityEmplaceComponent<Rotation>(e, entityManager.entityGetComponent<Rotation>(player));
        Acceleration a;
        a.value = entityManager.entityGetComponent<Rotation>(player).value * (Vec3::forward * 0.05);
        entityManager.entityEmplaceComponent<Acceleration>(e, a);
    }

    for (auto& [entity, acceleration] : entityManager.getComponents<Acceleration>())
    {
        auto& position = entityManager.entityGetComponent<Position>(entity).value;
        position += acceleration.value;
    }

    playerMovementSystem.update(*this, player);
    updateCamera();

    renderingSystem.update(entityManager, player, cameraSystem.view(), cameraSystem.projection());
}

void TestScene::registerInputActions()
{
    input.registerKeyboardButton("forward", KeyCode::W);
    input.registerKeyboardButton("back", KeyCode::S);
    input.registerKeyboardButton("left", KeyCode::A);
    input.registerKeyboardButton("right", KeyCode::D);

    input.registerKeyboardButton("jump", KeyCode::Space);
    input.registerKeyboardButton("crouch", KeyCode::LeftShift);

    input.registerKeyboardButton("exit", KeyCode::Escape);

    input.registerMouseButton("attack", MouseButton::Left);
}

void TestScene::registerComponents()
{
    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();
    entityManager.registerComponent<PlayerMovementComponent>();
    entityManager.registerComponent<RenderingComponent>();
    entityManager.registerComponent<Acceleration>();
}

void TestScene::createPlayer()
{
    player = entityManager.createEntity();
    entityManager.entityEmplaceComponent<Position>(player, Vec3(0, 0, -2));
    entityManager.entityEmplaceComponent<Rotation>(player, Quat::identity);
    entityManager.entityEmplaceComponent<PlayerMovementComponent>(player);
}

void TestScene::setupGraphics()
{
    glfwSetWindowPos(engine.window().handle(), 600, 400);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    engine.window().hideCursor();

    /*Gfx::setClearColor(Color(0.52f, 0.80f, 0.92f, 1.0f));*/
    Gfx::setClearColor(Color(0, 0, 0, 1.0f));
}

void TestScene::updateCamera()
{
    Vec2I windowSize = engine.window().getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    cameraSystem.update(entityManager, player, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
}
