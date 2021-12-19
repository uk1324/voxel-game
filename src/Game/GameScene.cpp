#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>

#include <iostream>

GameScene::GameScene(Engine& engine)
	: Scene(engine, 100)
    , shader("src/Game/Blocks/chunk.vert", "src/Game/Blocks/chunk.frag")
    , textureArray(16, 16, { "assets/textures/blocks/dirt.png", "assets/textures/blocks/stone.png", "assets/textures/blocks/cobblestone.png", "assets/textures/blocks/grass.png", "assets/textures/blocks/grass2.png" })
    , cameraSystem(90, 0.1f, 1000.0f)
    , skyboxShader("src/Game/cubemap.vert", "src/Game/cubemap.frag")
    , skybox( Gfx::CubeMapTexturePaths { "assets/textures/skybox/right.png", "assets/textures/skybox/left.png", "assets/textures/skybox/top.png", "assets/textures/skybox/bottom.png", "assets/textures/skybox/front.png", "assets/textures/skybox/back.png" })
{
    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();
    entityManager.registerComponent<PlayerMovementComponent>();

    playerMovementSystem.registerActions(input);

    player = entityManager.createEntity();
    entityManager.entityEmplaceComponent<Position>(player, Vec3(0, 40, -2));
    entityManager.entityEmplaceComponent<Rotation>(player, Quat::identity);
    entityManager.entityEmplaceComponent<PlayerMovementComponent>(player);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

    glEnable(GL_DEPTH_TEST);


    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    input.registerKeyboardButton("exit", KeyCode::Escape);

    Gfx::setClearColor(Color(0.52f, 0.80f, 0.92f, 1.0f));
}

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);

    playerMovementSystem.update(*this, player);

    
    Vec2I windowSize = engine.window().getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    cameraSystem.update(entityManager, player, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    chunkSystem.update(entityManager.entityGetComponent<Position>(player).value);

    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());

    shader.use();

    textureArray.bind();

    chunkSystem.m_vao.bind();
    for (const auto& chunk : chunkSystem.m_chunksToDraw)
    {
        Vec3I pos = chunk->pos;
        pos.x = -pos.x;
        pos.z = -pos.z;
        shader.setVec3I("chunkPos", pos);
        Gfx::drawTriangles(chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);
        //std::cout << chunk->pos << ' ' << chunk->vboByteOffset << ' ' << chunk->vertexCount << '\n';
    }

    glDepthFunc(GL_LEQUAL);

    glDepthMask(GL_FALSE);

    //glDisable(GL_CULL_FACE);

    skybox.bind();
    //skyboxVao.bind();
    Gfx::Primitives::cubeTrianglesVao->bind();

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    skyboxShader.setMat4("projection", cameraSystem.projection());
    Mat4 view = cameraSystem.view();
    view.removeTranslation();
    skyboxShader.setMat4("view", view);

    glFrontFace(GL_CW);

    Gfx::drawTriangles(0, 36);

    glFrontFace(GL_CCW);

    glDepthMask(GL_TRUE);
    //glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS);

    engine.window().hideCursor();

    // Maybe scale the line using a matrix in Debug::drawLine
}
