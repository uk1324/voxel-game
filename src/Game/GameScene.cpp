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

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static void addTextureOne(std::vector<Vec2>& tex)
{
    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 0.0f);
}

static void addTextureTwo(std::vector<Vec2>& tex)
{
    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 0.0f);
}

static void addCubeTop(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x, y + 1, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
    tex.emplace_back(0.0f, 1.0f);

    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
}

static void addCubeBottom(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 1.0f);
    tex.emplace_back(1.0f, 1.0f);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y, z);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
    tex.emplace_back(1.0f, 0.0f);
}

static void addCubeLeft(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 1.0f);
    tex.emplace_back(1.0f, 0.0f);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y + 1, z + 1);

    tex.emplace_back(1.0f, 0.0f);
    tex.emplace_back(0.0f, 1.0f);
    tex.emplace_back(1.0f, 1.0f);
}

static void addCubeRight(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 0.0f);
    tex.emplace_back(0.0f, 1.0f);

    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);

    tex.emplace_back(1.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
    tex.emplace_back(0.0f, 1.0f);
}

static void addCubeFront(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x, y + 1, z);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
    tex.emplace_back(0.0f, 1.0f);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y + 1, z);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
}

static void addCubeBack(std::vector<Vec3>& mesh, std::vector<Vec2>& tex, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(0.0f, 1.0f);
    tex.emplace_back(1.0f, 1.0f);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);

    tex.emplace_back(0.0f, 0.0f);
    tex.emplace_back(1.0f, 1.0f);
    tex.emplace_back(1.0f, 0.0f);
}

static bool isInBounds(size_t x, size_t y, size_t z)
{
    return ((x >= 0) && (x < Chunk::WIDTH))
        && ((y >= 0) && (y < Chunk::HEIGHT))
        && ((z >= 0) && (z < Chunk::DEPTH));
}

static std::vector<Vec3> meshFromChunk(Chunk& chunk, std::vector<Vec2>& tex)
{
    std::vector<Vec3> mesh;

    for (size_t x = 0; x < Chunk::WIDTH; x++)
    {
        for (size_t y = 0; y < Chunk::HEIGHT; y++)
        {
            for (size_t z = 0; z < Chunk::DEPTH; z++)
            {
                if (chunk(x, y, z).type != BlockType::Air)
                {
                    if (isInBounds(x, y + 1, z) == false || chunk(x, y + 1, z) == BlockType::Air)
                    {
                        addCubeTop(mesh, tex, x, y, z);
                    }
                    if (isInBounds(x, y - 1, z) == false || chunk(x, y - 1, z) == BlockType::Air)
                    {
                        addCubeBottom(mesh, tex, x, y, z);
                    }
                    if (isInBounds(x + 1, y, z) == false || chunk(x + 1, y, z) == BlockType::Air)
                    {
                        addCubeRight(mesh, tex, x, y, z);
                    }
                    if (isInBounds(x - 1, y, z) == false || chunk(x - 1, y, z) == BlockType::Air)
                    {
                        addCubeLeft(mesh, tex, x, y, z);
                    }
                    if (isInBounds(x, y, z + 1) == false || chunk(x, y, z + 1) == BlockType::Air)
                    {
                        addCubeBack(mesh, tex, x, y, z);
                    }
                    if (isInBounds(x, y, z - 1) == false || chunk(x, y, z - 1) == BlockType::Air)
                    {
                        addCubeFront(mesh, tex, x, y, z);
                    }
                }
            }
        }
    }

    return mesh;
}

size_t vertexCount;

GameScene::GameScene(Engine& engine)
	: Scene(engine)
    , shader([]{
        Shader vertexShader("../../../src/Game/shader.vert", ShaderType::Vertex);
        Shader fragmentShader("../../../src/Game/shader.frag", ShaderType::Fragment);
        ShaderProgram s;
        s.addShader(vertexShader);
        s.addShader(fragmentShader);
        s.link();
        return s;
    }())
    , texture("../../../dirt.png")
    , cameraSystem(800, 600, 90, 0.1f, 1000.0f)
{
    registerActions();

    for (size_t x = 0; x < Chunk::WIDTH; x++)
    {
        for (size_t y = 0; y < Chunk::HEIGHT; y++)
        {
            for (size_t z = 0; z < Chunk::DEPTH; z++)
            {
                chunk(x, z, z) = BlockType::Dirt;
            }
        }
    }

    player = &entityManager.addEntity();
    entityManager.entityAddComponent<Position>(*player, Vec3(0, 0, -2));
    entityManager.entityAddComponent<Rotation>(*player, Quat::identity);
    entityManager.entityAddComponent<PlayerMovementComponent>(*player);

    std::vector<Vec2> tex;
    std::vector<Vec3> mesh = meshFromChunk(chunk, tex);
    std::cout << tex.size() << ' ' << mesh.size() << '\n';
    vertexCount = mesh.size();
    
    vao.bind();
    
    meshVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, mesh.data(), sizeof(float) * mesh.size() * 3);
    meshVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    texVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, tex.data(), sizeof(float) * tex.size() * 2);
    texVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);

    texture.bind();

    glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    setClearColor(0, 0, 0, 1.0f);
}

#include <iostream>

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    clearBuffer(ClearModeBit::ColorBuffer | ClearModeBit::DepthBuffer);

    playerMovementSystem.update(*this, *player);
    cameraSystem.update(*player);

    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());
    shader.use();

    vao.bind();
    drawTriangles(vertexCount);
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
