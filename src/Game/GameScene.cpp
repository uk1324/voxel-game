#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

GameScene::GameScene(Engine& engine)
	: Scene(engine)
    , shader([]{
        Gfx::Shader vertexShader("../../../src/Game/Blocks/chunk.vert", Gfx::ShaderType::Vertex);
        Gfx::Shader fragmentShader("../../../src/Game/Blocks/chunk.frag", Gfx::ShaderType::Fragment);
        Gfx::ShaderProgram s;
        s.addShader(vertexShader);
        s.addShader(fragmentShader);
        s.link();
        return s;
    }())
    // , texture("../../../assets/textures/blocks/stone.png")
    , textureArray(16, 16, { "../../../assets/textures/blocks/dirt.png", "../../../assets/textures/blocks/stone.png", "../../../assets/textures/blocks/cobblestone.png", "../../../assets/textures/blocks/grass.png", "../../../assets/textures/blocks/grass2.png" })
    , cameraSystem(90, 0.1f, 1000.0f)
    , skyboxShader([] {
    Gfx::Shader vertexShader("../../../src/Game/cubemap.vert", Gfx::ShaderType::Vertex);
    Gfx::Shader fragmentShader("../../../src/Game/cubemap.frag", Gfx::ShaderType::Fragment);
    Gfx::ShaderProgram s;
    s.addShader(vertexShader);
    s.addShader(fragmentShader);
    s.link();
    return s;
        }())
        // Make the constructor take 6 arugments instead of std::array
    , skybox({ "../../../assets/textures/skybox/right.png", "../../../assets/textures/skybox/left.png", "../../../assets/textures/skybox/top.png", "../../../assets/textures/skybox/bottom.png", "../../../assets/textures/skybox/front.png", "../../../assets/textures/skybox/back.png" })
{
    registerActions();

    player = &entityManager.addEntity();
    entityManager.entityAddComponent<Position>(*player, Vec3(0, 40, -2));
    entityManager.entityAddComponent<Rotation>(*player, Quat::identity);
    entityManager.entityAddComponent<PlayerMovementComponent>(*player);

    textureArray.bind();
    shader.setInt("blockTextureArray", 0);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

    glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);


    Gfx::setClearColor(0.52, 0.80, 0.92, 1.0f);
}

#include <iostream>
#include <chrono>

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);

    playerMovementSystem.update(*this, *player);

    
    Vec2I windowSize = engine.window().getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    cameraSystem.update(*player, windowSize.x, windowSize.y);
    chunkSystem.update(player->getComponent<Position>().value);

    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());

    shader.use();

    textureArray.bind();

    for (const auto& chunk : chunkSystem.chunkMesh)
    {
        chunk.second.vao.bind();
        Vec3I pos = chunk.first;
        pos.x = -pos.x;
        pos.z = -pos.z;
        shader.setVec3I("chunkPos", pos);
        Gfx::drawTriangles(0, chunk.second.vertexCount);
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
    view.removeTransform();
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

void GameScene::registerActions()
{
    input.registerKeyboardButton("forward", KeyCode::W);
    input.registerKeyboardButton("back", KeyCode::S);
    input.registerKeyboardButton("left", KeyCode::A);
    input.registerKeyboardButton("right", KeyCode::D);

    input.registerKeyboardButton("jump", KeyCode::Space);
    input.registerKeyboardButton("crouch", KeyCode::LeftShift);

    input.registerKeyboardButton("exit", KeyCode::Escape);
}
