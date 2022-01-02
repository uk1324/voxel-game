#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>
#include <chrono>

#include <Game/Inventory/Inventory.hpp>

#include <string>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Game/Debug/Debug.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>

#include <iostream>

#include <math.h>
// Should timestep be fixed for things like chunk loading

GameScene::GameScene(Engine& engine)
    : Scene(engine, 100)
    , m_renderingSystem(*this)
    , m_playerMovementSystem(*this)
    , m_physicsSystem(*this)
{
    Debug::init(input, m_renderingSystem);

    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();

    m_player = entityManager.createEntity();
    entityManager.entityEmplaceComponent<Position>(m_player, Vec3(0, 40, 0));
    entityManager.entityEmplaceComponent<Rotation>(m_player, Quat::identity);
    entityManager.entityEmplaceComponent<PlayerMovementComponent>(m_player);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

    input.registerKeyboardButton("exit", KeyCode::X);
    input.registerKeyboardButton("test", KeyCode::F);
    input.registerKeyboardButton("test1", KeyCode::E);
    input.registerMouseButton("attack", MouseButton::Left);
    input.registerMouseButton("use", MouseButton::Right);

    input.registerKeyboardButton("pause", KeyCode::Escape);

    m_isCursorShown = false;
    engine.window().hideCursor();

    PhysicsAabbCollider collider;
    /*collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
    collider.size = Vec3(1.875, 1.875, 1.875);*/
    collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
    collider.halfSize = Vec3(0.6, 1.875, 0.6) / 2.0f;
    entityManager.entityEmplaceComponent<PhysicsAabbCollider>(m_player, collider);
    entityManager.entityEmplaceComponent<PhysicsVelocity>(m_player, PhysicsVelocity{ Vec3(0, 0, 0) });
    entityManager.entityEmplaceComponent<Grounded>(m_player, Grounded{ false });
}

void GameScene::update()
{
    auto start = std::chrono::high_resolution_clock::now();

    Debug::update(input);

    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    if (input.isButtonDown("pause"))
    {
        if (m_isCursorShown)
            engine.window().hideCursor();
        else
            engine.window().showCursor();
        m_isCursorShown = !m_isCursorShown;
    }

    const Vec3 playerPos = entityManager.entityGetComponent<Position>(m_player).value;
    const Quat playerRot = entityManager.entityGetComponent<Rotation>(m_player).value;
    const Vec2I windowSize = engine.window().getWindowSize();
    m_renderingSystem.update(
        static_cast<float>(windowSize.x), static_cast<float>(windowSize.y),
        playerPos, playerRot,
        entityManager,
        m_chunkSystem
    );

    m_inventory.render(m_chunkSystem.blockData, input, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    m_chunkSystem.update(entityManager.entityGetComponent<Position>(m_player).value);

    m_playerMovementSystem.update(*this, m_player);

    m_physicsSystem.update(time, entityManager, m_chunkSystem);

    for (auto point : points)
    {
        Debug::drawCube(Vec3(point) + Vec3(0.5), Vec3(1), Vec3(0, 0, 1));
    }


    Debug::drawLine(rayStart, rayEnd);

    if (input.isButtonDown("use"))
    {
    #define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
    #define FRAC0(x) (x - floorf(x))
    #define FRAC1(x) (1 - x + floorf(x))

        Vec3 ray_start = entityManager.entityGetComponent<Position>(m_player).value;
        const Quat& rotation = entityManager.entityGetComponent<Rotation>(m_player).value;
        const Vec3& direction = rotation * Vec3::forward;
        Vec3 ray_end = ray_start + direction * 5;;
        rayStart = ray_start;
        rayEnd = ray_end;

        float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;
        Vec3 voxel;

        float x1 = rayStart.x, y1 = rayStart.y, z1 = rayStart.z; // start point   
        float x2 = rayEnd.x, y2 = rayEnd.y, z2 = rayEnd.z; // end point   

        int dx = SIGN(x2 - x1) * Block::SIZE;
        if (dx != 0) tDeltaX = fmin(dx / (x2 - x1), 10000000.0f); else tDeltaX = 10000000.0f;
        if (dx > 0) tMaxX = tDeltaX * FRAC1(x1); else tMaxX = tDeltaX * FRAC0(x1);
        voxel.x = floor(x1);

        int dy = SIGN(y2 - y1);
        if (dy != 0) tDeltaY = fmin(dy / (y2 - y1), 10000000.0f); else tDeltaY = 10000000.0f;
        if (dy > 0) tMaxY = tDeltaY * FRAC1(y1); else tMaxY = tDeltaY * FRAC0(y1);
        voxel.y = floor(y1);

        int dz = SIGN(z2 - z1);
        if (dz != 0) tDeltaZ = fmin(dz / (z2 - z1), 10000000.0f); else tDeltaZ = 10000000.0f;
        if (dz > 0) tMaxZ = tDeltaZ * FRAC1(z1); else tMaxZ = tDeltaZ * FRAC0(z1);
        voxel.z = floor(z1);
        points.clear();
        points.push_back(voxel);

        while (true) {
            if (m_chunkSystem.get(voxel.x, voxel.y, voxel.z) != BlockType::Air)
            {
                //points.push_back(current_voxel);
                Block b;
                b.type = BlockType::Debug;
                //chunkSystem.set(current_voxel.x, current_voxel.y, current_voxel.z, b);
                Vec3 current_voxel;
                try
                {
                    current_voxel = points.at(points.size() - 2);
                    if (m_chunkSystem.get(current_voxel.x, current_voxel.y, current_voxel.z).type == BlockType::Air)
                    {
                        m_chunkSystem.set(current_voxel.x, current_voxel.y, current_voxel.z, b);
                    }
                }
                catch (const std::exception&)
                {
                    std::cout << ":(\n";
                }
                break;
            }

            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    voxel.x += dx;
                    tMaxX += tDeltaX;
                }
                else {
                    voxel.z += dz;
                    tMaxZ += tDeltaZ;
                }
            }
            else {
                if (tMaxY < tMaxZ) {
                    voxel.y += dy;
                    tMaxY += tDeltaY;
                }
                else {
                    voxel.z += dz;
                    tMaxZ += tDeltaZ;
                }
            }
            points.push_back(voxel);
            if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
            // process voxel here
        }
    }



    if (input.isButtonDown("attack"))
    {
#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

        Vec3 ray_start = entityManager.entityGetComponent<Position>(m_player).value;
        const Quat& rotation = entityManager.entityGetComponent<Rotation>(m_player).value;
        const Vec3& direction = rotation * Vec3::forward;
        Vec3 ray_end = ray_start + direction * 5;;
        rayStart = ray_start;
        rayEnd = ray_end;

        float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;
        Vec3 voxel;

        float x1 = rayStart.x, y1 = rayStart.y, z1 = rayStart.z; // start point   
        float x2 = rayEnd.x, y2 = rayEnd.y, z2 = rayEnd.z; // end point   

        int dx = SIGN(x2 - x1) * Block::SIZE;
        if (dx != 0) tDeltaX = fmin(dx / (x2 - x1), 10000000.0f); else tDeltaX = 10000000.0f;
        if (dx > 0) tMaxX = tDeltaX * FRAC1(x1); else tMaxX = tDeltaX * FRAC0(x1);
        voxel.x = floor(x1);

        int dy = SIGN(y2 - y1);
        if (dy != 0) tDeltaY = fmin(dy / (y2 - y1), 10000000.0f); else tDeltaY = 10000000.0f;
        if (dy > 0) tMaxY = tDeltaY * FRAC1(y1); else tMaxY = tDeltaY * FRAC0(y1);
        voxel.y = floor(y1);

        int dz = SIGN(z2 - z1);
        if (dz != 0) tDeltaZ = fmin(dz / (z2 - z1), 10000000.0f); else tDeltaZ = 10000000.0f;
        if (dz > 0) tMaxZ = tDeltaZ * FRAC1(z1); else tMaxZ = tDeltaZ * FRAC0(z1);
        voxel.z = floor(z1);
        points.clear();
        points.push_back(voxel);

        while (true) {
            if (m_chunkSystem.get(voxel.x, voxel.y, voxel.z) != BlockType::Air)
            {
                //points.push_back(current_voxel);
                Block b;
                b.type = BlockType::Air;
                m_chunkSystem.set(voxel.x, voxel.y, voxel.z, b);
                break;
            }

            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    voxel.x += dx;
                    tMaxX += tDeltaX;
                }
                else {
                    voxel.z += dz;
                    tMaxZ += tDeltaZ;
                }
            }
            else {
                if (tMaxY < tMaxZ) {
                    voxel.y += dy;
                    tMaxY += tDeltaY;
                }
                else {
                    voxel.z += dz;
                    tMaxZ += tDeltaZ;
                }
            }
            points.push_back(voxel);
            if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1) break;
            // process voxel here
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    glfwSetWindowTitle(engine.window().handle(), (std::string("frame time: ") + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())).c_str());
}
