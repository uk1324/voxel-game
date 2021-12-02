#include "GameScene.hpp"
#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>
#include <Math/Mat4.hpp>
#include <Math/Angles.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Math/Box.hpp>
#include <Engine/Debug/Drawing.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

size_t vertexCount;

GameScene::GameScene(Engine& engine)
	: Scene(engine)
    , shader([]{
        Shader vertexShader("../../../src/Game/Blocks/chunk.vert", ShaderType::Vertex);
        Shader fragmentShader("../../../src/Game/Blocks/chunk.frag", ShaderType::Fragment);
        ShaderProgram s;
        s.addShader(vertexShader);
        s.addShader(fragmentShader);
        s.link();
        return s;
    }())
    // , texture("../../../assets/textures/blocks/stone.png")
    , textureArray(16, 16, { "../../../assets/textures/blocks/dirt.png", "../../../assets/textures/blocks/stone.png", "../../../assets/textures/blocks/cobblestone.png", "../../../assets/textures/blocks/grass.png", "../../../assets/textures/blocks/grass2.png" })
    , cameraSystem(800, 600, 90, 0.1f, 1000.0f)
{
    registerActions();
    
    //for (size_t z = 0; z < Chunk::WIDTH; z++)
    //{
    //    for (size_t y = 0; y < Chunk::HEIGHT; y++)
    //    {
    //        for (size_t x = 0; x < Chunk::DEPTH; x++)
    //        {
    //            std::cout << z * (16 * 16) + y * 16 + x << '\n';
    //            chunk(x, z, z) = static_cast<BlockType>((rand() % 3) + 1);
    //        }
    //    }
    //}

    player = &entityManager.addEntity();
    entityManager.entityAddComponent<Position>(*player, Vec3(0, 40, -2));
    entityManager.entityAddComponent<Rotation>(*player, Quat::identity);
    entityManager.entityAddComponent<PlayerMovementComponent>(*player);

    //std::vector<GLuint> vertices = meshFromChunk(*chunkSystem.chunks[Vec3I(0, 0, 0)]);
    //vertexCount = vertices.size();
    //vao.bind();
    //vbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, vertices.data(), sizeof(GLuint) * vertices.size());
    //vbo.bind(BufferBindTarget::ArrayBuffer);
    //glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
    //glEnableVertexAttribArray(0);

    textureArray.bind();
    shader.setInt("blockTextureArray", 0);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

    glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    setClearColor(0.52, 0.80, 0.92, 1.0f);
}

#include <iostream>
#include <chrono>

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    //Box box(Vec3(-1, 0, -1), Vec3(1, 1, 1));
    //const Vec3& playerPos = player->getComponent<Position>().value;
    //if (box.contains(playerPos))
    //{
    //    std::cout << "collision ";
    //}
    //std::cout << playerPos << "\n";

    clearBuffer(ClearModeBit::ColorBuffer | ClearModeBit::DepthBuffer);

    playerMovementSystem.update(*this, *player);

    // Stress test
    //Vec3& playerPos = player->getComponent<Position>().value;
    //playerPos.x = rand();
    //playerPos.y = rand();
    //playerPos.z = rand();
    int width, height;
    engine.window().getWindowSize(width, height);
    glViewport(0, 0, width, height);
    cameraSystem.update(*player, width, height);
    chunkSystem.update(player->getComponent<Position>().value);

    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());

    shader.use();

    for (const auto& chunk : chunkSystem.chunkMesh)
    {
        chunk.second.vao.bind();
        Vec3I pos = chunk.first;
        pos.x = -pos.x;
        pos.z = -pos.z;
        shader.setVec3I("chunkPos", pos);
        drawTriangles(chunk.second.vertexCount);
    }

    //vao.bind();
    //drawTriangles(vertexCount);

    //int count = 10;
    //float angle = (PI<float> * 2)/ static_cast<float>(count);
    //for (float i = 0; i < count; i++)
    //{
    //    Debug::drawLine(Vec3(0, 0, 0), Vec3(cos(angle * i + glfwGetTime()), sin(angle * i + glfwGetTime()), 0), Vec3(1, 0, 0));
    //}
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
