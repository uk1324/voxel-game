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

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

size_t Left = 0;
size_t Right = 1;
size_t Top = 2;
size_t Bottom = 3;
size_t Back = 4;
size_t Front = 5;


static void addIndex(std::vector<GLuint>& index, Block block)
{
    index.push_back(static_cast<GLuint>(block.type) - 1);
    index.push_back(static_cast<GLuint>(block.type) - 1);
    index.push_back(static_cast<GLuint>(block.type) - 1);
    index.push_back(static_cast<GLuint>(block.type) - 1);
    index.push_back(static_cast<GLuint>(block.type) - 1);
    index.push_back(static_cast<GLuint>(block.type) - 1);
}

static void addNormals(std::vector<GLuint>& normals, GLuint value)
{
    normals.push_back(value);
    normals.push_back(value);
    normals.push_back(value);
    normals.push_back(value);
    normals.push_back(value);
    normals.push_back(value);
}

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

static void addVertex(std::vector<GLuint>& vertices, size_t x, size_t y, size_t z, Block textureIndex, size_t texturePosIndex)
{
    GLuint vertex;

    static constexpr int Y_OFFSET = 5;
    static constexpr int Z_OFFSET = Y_OFFSET + 5;
    static constexpr int TEXTURE_INDEX_OFFSET = Z_OFFSET + 5;
    static constexpr int TEXTURE_POS_INDEX_OFFSET = TEXTURE_INDEX_OFFSET + 10;

    vertex = x;
    vertex |= (y << Y_OFFSET);
    vertex |= (z << Z_OFFSET);
    vertex |= ((static_cast<GLuint>(textureIndex.type) - 1) << TEXTURE_INDEX_OFFSET);
    vertex |= (texturePosIndex << TEXTURE_POS_INDEX_OFFSET);

    vertices.push_back(vertex);
}

static void addCubeTop(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);

    addNormals(normals, Top);

    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x, y + 1, z + 1);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    //tex.emplace_back(0.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(3);
    tex.push_back(2);

    addVertex(vertices, x, y + 1, z, block, 0);
    addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
    addVertex(vertices, x, y + 1, z + 1, block, 2);

    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x + 1, y + 1, z + 1);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(1);
    tex.push_back(3);

    addVertex(vertices, x, y + 1, z, block, 0);
    addVertex(vertices, x + 1, y + 1, z, block, 1);
    addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
}

static void addCubeBottom(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);
    addNormals(normals, Bottom);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);

    // tex.emplace_back(0.0f, 0.0f);
    // tex.emplace_back(0.0f, 1.0f);
    // tex.emplace_back(1.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(2);
    tex.push_back(3);

    addVertex(vertices, x, y, z, block, 0);
    addVertex(vertices, x, y, z + 1, block, 2);
    addVertex(vertices, x + 1, y, z + 1, block, 3);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y, z);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    //tex.emplace_back(1.0f, 0.0f);
    tex.push_back(0);
    tex.push_back(3);
    tex.push_back(1);

    addVertex(vertices, x, y, z, block, 0);
    addVertex(vertices, x + 1, y, z + 1, block, 3);
    addVertex(vertices, x + 1, y, z, block, 1);
}

static void addCubeLeft(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);
    addNormals(normals, Left);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y, z + 1);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(0.0f, 1.0f);
    //tex.emplace_back(1.0f, 0.0f);
    tex.push_back(0);
    tex.push_back(2);
    tex.push_back(1);

    addVertex(vertices, x, y, z, block, 0);
    addVertex(vertices, x, y + 1, z, block, 2);
    addVertex(vertices, x, y, z + 1, block, 1);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z);
    mesh.emplace_back(x, y + 1, z + 1);

    //tex.emplace_back(1.0f, 0.0f);
    //tex.emplace_back(0.0f, 1.0f);
    //tex.emplace_back(1.0f, 1.0f);
    tex.push_back(1);
    tex.push_back(2);
    tex.push_back(3);

    addVertex(vertices, x, y, z + 1, block, 1);
    addVertex(vertices, x, y + 1, z, block, 2);
    addVertex(vertices, x, y + 1, z + 1, block, 3);
}

static void addCubeRight(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);
    addNormals(normals, Right);

    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 0.0f);
    //tex.emplace_back(0.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(1);
    tex.push_back(2);

    addVertex(vertices, x + 1, y, z, block, 0);
    addVertex(vertices, x + 1, y, z + 1, block, 1);
    addVertex(vertices, x + 1, y + 1, z, block, 2);

    mesh.emplace_back(x + 1, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z);

    //tex.emplace_back(1.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    //tex.emplace_back(0.0f, 1.0f);
    tex.push_back(1);
    tex.push_back(3);
    tex.push_back(2);

    addVertex(vertices, x + 1, y, z + 1, block, 1);
    addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
    addVertex(vertices, x + 1, y + 1, z, block, 2);
}

static void addCubeFront(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);
    addNormals(normals, Front);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y + 1, z);
    mesh.emplace_back(x, y + 1, z);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    //tex.emplace_back(0.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(3);
    tex.push_back(2);

    addVertex(vertices, x, y, z, block, 0);
    addVertex(vertices, x + 1, y + 1, z, block, 3);
    addVertex(vertices, x, y + 1, z, block, 2);

    mesh.emplace_back(x, y, z);
    mesh.emplace_back(x + 1, y, z);
    mesh.emplace_back(x + 1, y + 1, z);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(1);
    tex.push_back(3);

    addVertex(vertices, x, y, z, block, 0);
    addVertex(vertices, x + 1, y, z, block, 1);
    addVertex(vertices, x + 1, y + 1, z, block, 3);
}

static void addCubeBack(Block block, std::vector<Vec3>& mesh, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
    addIndex(index, block);
    addNormals(normals, Back);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x, y + 1, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(0.0f, 1.0f);
    //tex.emplace_back(1.0f, 1.0f);
    tex.push_back(0);
    tex.push_back(2);
    tex.push_back(3);

    addVertex(vertices, x, y, z + 1, block, 0);
    addVertex(vertices, x, y + 1, z + 1, block, 2);
    addVertex(vertices, x + 1, y + 1, z + 1, block, 3);

    mesh.emplace_back(x, y, z + 1);
    mesh.emplace_back(x + 1, y + 1, z + 1);
    mesh.emplace_back(x + 1, y, z + 1);

    //tex.emplace_back(0.0f, 0.0f);
    //tex.emplace_back(1.0f, 1.0f);
    //tex.emplace_back(1.0f, 0.0f);
    tex.push_back(0);
    tex.push_back(3);
    tex.push_back(1);

    addVertex(vertices, x, y, z + 1, block, 0);
    addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
    addVertex(vertices, x + 1, y, z + 1, block, 1);
}

static bool isInBounds(size_t x, size_t y, size_t z)
{
    return ((x >= 0) && (x < Chunk::WIDTH))
        && ((y >= 0) && (y < Chunk::HEIGHT))
        && ((z >= 0) && (z < Chunk::DEPTH));
}

static std::vector<Vec3> meshFromChunk(Chunk& chunk, std::vector<GLuint>& tex, std::vector<GLuint>& index, std::vector<GLuint>& normals, std::vector<GLuint>& vertices)
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
                    Block block = chunk(x, y, z);
                    if (isInBounds(x, y + 1, z) == false || chunk(x, y + 1, z) == BlockType::Air)
                    {
                        addCubeTop(block, mesh, tex, index, normals, vertices, x, y, z);
                    }
                    if (isInBounds(x, y - 1, z) == false || chunk(x, y - 1, z) == BlockType::Air)
                    {
                        addCubeBottom(block, mesh, tex, index, normals, vertices, x, y, z);
                    }
                    if (isInBounds(x + 1, y, z) == false || chunk(x + 1, y, z) == BlockType::Air)
                    {
                        addCubeRight(block, mesh, tex, index, normals, vertices, x, y, z);
                    }
                    if (isInBounds(x - 1, y, z) == false || chunk(x - 1, y, z) == BlockType::Air)
                    {
                        addCubeLeft(block, mesh, tex, index, normals, vertices, x, y, z);
                    }
                    if (isInBounds(x, y, z + 1) == false || chunk(x, y, z + 1) == BlockType::Air)
                    {
                        addCubeBack(block, mesh, tex, index, normals, vertices, x, y, z);
                    }
                    if (isInBounds(x, y, z - 1) == false || chunk(x, y, z - 1) == BlockType::Air)
                    {
                        addCubeFront(block, mesh, tex, index, normals, vertices, x, y, z);
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
        Shader vertexShader("../../../src/Game/Blocks/chunk.vert", ShaderType::Vertex);
        Shader fragmentShader("../../../src/Game/Blocks/chunk.frag", ShaderType::Fragment);
        ShaderProgram s;
        s.addShader(vertexShader);
        s.addShader(fragmentShader);
        s.link();
        return s;
    }())
    // , texture("../../../assets/textures/blocks/stone.png")
    , textureArray(16, 16, { "../../../assets/textures/blocks/dirt.png", "../../../assets/textures/blocks/stone.png", "../../../assets/textures/blocks/cobblestone.png" })
    , cameraSystem(800, 600, 90, 0.1f, 1000.0f)
{
    registerActions();
    
    for (size_t x = 0; x < Chunk::WIDTH; x++)
    {
        for (size_t y = 0; y < Chunk::HEIGHT; y++)
        {
            for (size_t z = 0; z < Chunk::DEPTH; z++)
            {
                chunk(x, z, z) = static_cast<BlockType>((rand() % 3) + 1);
            }
        }
    }

    player = &entityManager.addEntity();
    entityManager.entityAddComponent<Position>(*player, Vec3(0, 0, -2));
    entityManager.entityAddComponent<Rotation>(*player, Quat::identity);
    entityManager.entityAddComponent<PlayerMovementComponent>(*player);

    std::vector<GLuint> tex;
    std::vector<GLuint> index;
    std::vector<GLuint> normals;
    std::vector<GLuint> vertices;
    std::vector<Vec3> mesh = meshFromChunk(chunk, tex, index, normals, vertices);
    std::cout << "vbo sizes: " << vertices.size() << ' ' << tex.size() << ' ' << mesh.size() << ' ' << index.size() << '\n';
    //vertexCount = mesh.size();
    vertexCount = vertices.size();
    
    vao.bind();
    
    meshVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, mesh.data(), sizeof(float) * mesh.size() * 3);
    meshVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    texVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, tex.data(), sizeof(GLuint) * tex.size());
    texVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
    glEnableVertexAttribArray(1);

    indexVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, index.data(), sizeof(GLuint) * index.size());
    indexVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
    glEnableVertexAttribArray(2);

    normalVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, normals.data(), sizeof(GLuint) * normals.size());
    normalVbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
    glEnableVertexAttribArray(3);

    vbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, vertices.data(), sizeof(GLuint) * vertices.size());
    vbo.bind(BufferBindTarget::ArrayBuffer);
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
    glEnableVertexAttribArray(4);

    //texture.bind();
    textureArray.bind();
    shader.setInt("blockTextureArray", 0);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

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

    Box box(Vec3(-1, 0, -1), Vec3(1, 1, 1));
    const Vec3& playerPos = player->getComponent<Position>().value;
    if (box.contains(playerPos))
    {
        std::cout << "collision ";
    }
    std::cout << playerPos << "\n";

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
