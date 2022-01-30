//#include <TestScene/TestScene.hpp>
//#include <Engine/Engine.hpp>
//#include <Game/Components/Position.hpp>
//#include <Game/Components/Rotation.hpp>
//
//#include <iostream>
//
//struct Acceleration
//{
//    Vec3 value;
//};
//
//TestScene::TestScene(Engine& engine)
//    : Scene(engine, 100)
//    , cameraSystem(90.0f, 0.1f, 1000.0f)
//    , playerMovementSystem(*this)
//{
//    registerInputActions();
//    registerComponents();
//    createPlayer();
//    setupGraphics();
//
//    auto random = []()
//    {
//        return (((rand() % 1000) / 1000.0f));
//    };
//
//    for (int i = 0; i < 10; i++)
//    {
//        Entity entity = entityManager.createEntity();
//        Vec3 position(random() * 15, random() * 15, random() * 15);
//        entityManager.addComponent(entity, position);
//        Quat rotation(random(), Vec3(random(), random(), random()).normalized());
//        entityManager.addComponent(entity, rotation);
//        entityManager.addComponent(entity, RenderingComponent{});
//    }
//
//    //entity = entityManager.createEntity();
//    //entityManager.addComponent<Position>(entity, Vec3());
//    //entityManager.addComponent<Rotation>(entity, Quat::identity);
//    //entityManager.addComponent<RenderingComponent>(entity);
//}
//
//void TestScene::update()
//{
//    if (input.isButtonDown("exit"))
//    {
//        engine.stop();
//    }
//
//    entityManager.getComponent<Position>(entity) = entityManager.getComponent<Position>(player).value + Vec3(0, 0, 5);
//    entityManager.getComponent<Rotation>(entity) = entityManager.getComponent<Rotation>(player);
//
//    if (input.isButtonDown("attack"))
//    {
//        Entity e = entityManager.createEntity();
//        entityManager.addComponent(e, RenderingComponent{});
//        entityManager.addComponent(e, entityManager.getComponent<Position>(player));
//        entityManager.addComponent(e, entityManager.getComponent<Rotation>(player));
//        Acceleration a;
//        a.value = entityManager.getComponent<Rotation>(player).value * (Vec3::forward * 0.05);
//        entityManager.addComponent(e, a);
//    }
//
//    for (auto& [entity, acceleration] : entityManager.getComponents<Acceleration>())
//    {
//        auto& position = entityManager.getComponent<Position>(entity).value;
//        position += acceleration.value;
//    }
//
//    //playerMovementSystem.update(*this, player);
//    updateCamera();
//
//    renderingSystem.update(entityManager, player, cameraSystem.view(), cameraSystem.projection());
//}
//
//void TestScene::registerInputActions()
//{
//    input.registerKeyboardButton("forward", KeyCode::W);
//    input.registerKeyboardButton("back", KeyCode::S);
//    input.registerKeyboardButton("left", KeyCode::A);
//    input.registerKeyboardButton("right", KeyCode::D);
//
//    input.registerKeyboardButton("jump", KeyCode::Space);
//    input.registerKeyboardButton("crouch", KeyCode::LeftShift);
//
//    input.registerKeyboardButton("exit", KeyCode::Escape);
//
//    input.registerMouseButton("attack", MouseButton::Left);
//}
//
//void TestScene::registerComponents()
//{
//    entityManager.registerComponent<Position>();
//    entityManager.registerComponent<Rotation>();
//    entityManager.registerComponent<PlayerMovementComponent>();
//    entityManager.registerComponent<RenderingComponent>();
//    entityManager.registerComponent<Acceleration>();
//}
//
//void TestScene::createPlayer()
//{
//    player = entityManager.createEntity();
//    entityManager.addComponent(player, Position{ Vec3(0, 0, -2) });
//    entityManager.addComponent(player, Rotation{ Quat::identity });
//    entityManager.addComponent(player, PlayerMovementComponent{});
//}
//
//void TestScene::setupGraphics()
//{
//    glfwSetWindowPos(engine.window().handle(), 600, 400);
//    glEnable(GL_DEPTH_TEST);
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);
//    engine.window().hideCursor();
//
//    /*setClearColor(Color(0.52f, 0.80f, 0.92f, 1.0f));*/
//    glClearColor(0, 0, 0, 1.0f);
//    //setClearColor(Color(0, 0, 0, 1.0f));
//}
//
//void TestScene::updateCamera()
//{
//    Vec2I windowSize = engine.window().getWindowSize();
//    glViewport(0, 0, windowSize.x, windowSize.y);
//    cameraSystem.update(entityManager, player, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
//}
