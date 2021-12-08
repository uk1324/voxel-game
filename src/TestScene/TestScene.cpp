#include "TestScene.hpp"
#include "TestScene.hpp"
#include "TestScene.hpp"
#include <TestScene/TestScene.hpp>
#include <Engine/Engine.hpp>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>


TestScene::TestScene(Engine& engine)
    : Scene(engine, 100)
    , cameraSystem(90.0f, 0.1f, 1000.0f)
{
    Gfx::Shader vertex("src/TestScene/shader.vert", Gfx::ShaderType::Vertex);
    Gfx::Shader fragment("src/TestScene/shader.frag", Gfx::ShaderType::Fragment);
    shader.addShader(vertex);
    shader.addShader(fragment);
    shader.link();

    registerInputActions();
    registerComponents();
    createPlayer();
    setupGraphics();

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void TestScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    playerMovementSystem.update(*this, player);
    updateCamera();

    Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);
    Gfx::Primitives::cubeTrianglesVao->bind();
    shader.setMat4("projection", cameraSystem.projection());
    shader.setMat4("view", cameraSystem.view());
    shader.use();
    Gfx::drawTriangles(0, 36);
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
}

void TestScene::registerComponents()
{
    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();
    entityManager.registerComponent<PlayerMovementComponent>();
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
    Gfx::setClearColor(0.52f, 0.80f, 0.92f, 1.0f);
}

void TestScene::updateCamera()
{
    Vec2I windowSize = engine.window().getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    cameraSystem.update(entityManager, player, windowSize.x, windowSize.y);
}
