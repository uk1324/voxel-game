#include "GameScene.hpp"
#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>
#include <Math/Mat4.hpp>
#include <Math/Angles.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>

#include <iostream>

#include <stb_image.h>

// Should player be a pointer or a reference?

static void addCubeTop(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x, y + 1, z + 1);

    mesh.emplace_back(x, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z + 1);
}

static void addCubeBottom(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y, z);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y, z);
}

static void addCubeLeft(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y, z + 1);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y + 1, z + 1);
}

static void addCubeRight(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);

    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);
}

static void addCubeFront(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x, y + 1, z);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y + 1, z);
}

static void addCubeBack(std::vector<Vec3>& mesh, size_t x, size_t y, size_t z)
{
    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);
}

static bool isInBounds(size_t x, size_t y, size_t z)
{
    return ((x >= 0) && (x < Chunk::WIDTH))
        && ((y >= 0) && (y < Chunk::HEIGHT))
        && ((z >= 0) && (z < Chunk::DEPTH));
}

static std::vector<Vec3> meshFromChunk(Chunk& chunk)
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
                        addCubeTop(mesh, x, y, z);
                    }
                    if (isInBounds(x, y - 1, z) == false || chunk(x, y - 1, z) == BlockType::Air)
                    {
                        addCubeBottom(mesh, x, y, z);
                    }
                    if (isInBounds(x + 1, y, z) == false || chunk(x + 1, y, z) == BlockType::Air)
                    {
                        addCubeRight(mesh, x, y, z);
                    }
                    if (isInBounds(x - 1, y, z) == false || chunk(x - 1, y, z) == BlockType::Air)
                    {
                        addCubeLeft(mesh, x, y, z);
                    }
                    if (isInBounds(x, y, z + 1) == false || chunk(x, y, z + 1) == BlockType::Air)
                    {
                        addCubeBack(mesh, x, y, z);
                    }
                    if (isInBounds(x, y, z - 1) == false || chunk(x, y, z - 1) == BlockType::Air)
                    {
                        addCubeFront(mesh, x, y, z);
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
    ,cameraSystem(800, 600, 90, 0.1f, 1000.0f)
{
    registerActions();

    for (size_t x = 0; x < Chunk::WIDTH; x++)
    {
        for (size_t y = 0; y < Chunk::HEIGHT; y++)
        {
            for (size_t z = 0; z < Chunk::DEPTH; z++)
            {
                chunk(x, 1, z) = BlockType::Dirt;
            }
        }
    }
    //chunk(0, 0, 0) = BlockType::Dirt;

    glEnable(GL_DEPTH_TEST);

    player = &entityManager.addEntity();
    entityManager.entityAddComponent<Position>(*player, Vec3(0, 0, -2));
    entityManager.entityAddComponent<Rotation>(*player, Quat::identity);
    entityManager.entityAddComponent<PlayerMovementComponent>(*player);

    Mesh mesh;
    //mesh.vertices = std::vector<Vec3>({
    //    Vec3(-0.5f, -0.5f, -0.5f),
    //    Vec3(0.5f, -0.5f, -0.5f),
    //    Vec3(0.5f, 0.5f, -0.5f)
    //});
    mesh.vertices = meshFromChunk(chunk);
    //std::cout << mesh.vertices.size() << '\n';
    vertexCount = mesh.vertices.size();

    model = Model(mesh);
    VAO = model.vao;


    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //stbi_set_flip_vertically_on_load(true);

    //unsigned int texture;
    //glGenTextures(1, &texture);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //// set the texture wrapping/filtering options (on the currently bound texture object)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load and generate the texture
    //int width, height, nrChannels;
    //unsigned char* data = stbi_load("../../../container.jpg", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    //std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);

    //glBindTexture(GL_TEXTURE_2D, texture);
    //glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
}

#include <iostream>

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    playerMovementSystem.update(*this, *player);
    cameraSystem.update(*player);


    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());
    shader.use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
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
