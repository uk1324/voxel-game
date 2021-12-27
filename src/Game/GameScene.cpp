#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>
#include <chrono>

#include <string>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>

#include <Engine/Debug/Drawing.hpp>

#include <iostream>

#include <math.h>
// Should timestep be fixed for things like chunk loading

GameScene::GameScene(Engine& engine)
    : Scene(engine, 100)
    , m_renderingSystem(*this)
    , m_physicsSystem(*this)
{
    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();
    entityManager.registerComponent<PlayerMovementComponent>();

    playerMovementSystem.registerActions(input);

    player = entityManager.createEntity();
    //entityManager.entityEmplaceComponent<Position>(player, Vec3(0, 40, -2));
    entityManager.entityEmplaceComponent<Position>(player, Vec3(1000, 40, 1000));
    //entityManager.entityEmplaceComponent<Position>(player, Vec3(0, 0, 0));
    entityManager.entityEmplaceComponent<Rotation>(player, Quat::identity);
    entityManager.entityEmplaceComponent<PlayerMovementComponent>(player);

    glfwSetWindowPos(engine.window().handle(), 600, 400);

    input.registerKeyboardButton("exit", KeyCode::X);
    input.registerKeyboardButton("test", KeyCode::F);
    input.registerKeyboardButton("test1", KeyCode::E);
    input.registerMouseButton("attack", MouseButton::Left);
    input.registerMouseButton("use", MouseButton::Right);

    input.registerKeyboardButton("toggleCursor", KeyCode::Escape);

    //glEnable(GL_PROGRAM_POINT_SIZE);
    isCursorShown = false;
    engine.window().hideCursor();

    //Entity e = entityManager.createEntity();
    //entityManager.entityAddComponent<Position>(e, Vec3(1000, 35, 1000));
    PhysicsAabbCollider collider;
    /*collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
    collider.size = Vec3(0.625, 1.875, 0.625);*/
    collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
    collider.size = Vec3(1.875, 1.875, 1.875);
    //entityManager.entityEmplaceComponent<PhysicsAabbCollider>(e, collider);
    //entityManager.entityEmplaceComponent<PhysicsVelocity>(e, PhysicsVelocity{ Vec3(0, 0, 0) });

    entityManager.entityEmplaceComponent<PhysicsAabbCollider>(player, collider);
    entityManager.entityEmplaceComponent<PhysicsVelocity>(player, PhysicsVelocity{ Vec3(0, 0, 0) });
    entityManager.entityEmplaceComponent<Grounded>(player, Grounded{ false });


    //glPointSize(10);'
}

void GameScene::update()
{
    auto start = std::chrono::high_resolution_clock::now();
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    if (input.isButtonDown("toggleCursor"))
    {
        if (isCursorShown)
            engine.window().hideCursor();
        else
            engine.window().showCursor();
        isCursorShown = !isCursorShown;
    }

    const Vec3 playerPos = entityManager.entityGetComponent<Position>(player).value;
    const Quat playerRot = entityManager.entityGetComponent<Rotation>(player).value;
    const Vec2I windowSize = engine.window().getWindowSize();
    m_renderingSystem.update(
        static_cast<float>(windowSize.x), static_cast<float>(windowSize.y),
        playerPos, playerRot,
        entityManager,
        chunkSystem
    );

    chunkSystem.update(entityManager.entityGetComponent<Position>(player).value);

    if (input.isButtonDown("test1"))
    {
        chunkSystem.remesh();
    }

    playerMovementSystem.update(*this, player);

    m_physicsSystem.update(entityManager, chunkSystem);

    if (input.isButtonDown("test"))
    {
        //chunkSystem.set(1006, 29, 993, BlockType::Cobblestone);
        Entity e = entityManager.createEntity();
        entityManager.entityAddComponent<Position>(e, Vec3(playerPos));
        PhysicsAabbCollider collider;
        collider.centerOffset = Vec3(0, 0, 0);
        collider.size = Vec3(1, 1, 1);
        entityManager.entityEmplaceComponent<PhysicsAabbCollider>(e, collider);
        entityManager.entityEmplaceComponent<PhysicsVelocity>(e, PhysicsVelocity{ Vec3(0, 0, 0) });
    }

    for (auto point : points)
    {
        Debug::drawCube(Vec3(point) + Vec3(0.5), Vec3(1), Vec3(0, 0, 1));
    }

    if (input.isButtonDown("use"))
    {
        Vec3 ray_start = entityManager.entityGetComponent<Position>(player).value;
        const Quat& rotation = entityManager.entityGetComponent<Rotation>(player).value;
        const Vec3& direction = rotation * Vec3::forward;
        Vec3 ray_end = ray_start + direction * 5;;

        std::vector<Vec3> visited_voxels;

        // This id of the first/current voxel hit by the ray.
        // Using floor (round down) is actually very important,
        // the implicit int-casting will round up for negative numbers.
        Vec3 current_voxel(std::floor(ray_start.x),
            std::floor(ray_start.y),
            std::floor(ray_start.z));
        //std::cout << current_voxel << '\n';

        Vec3 last_voxel(std::floor(ray_end.x),
            std::floor(ray_end.y),
            std::floor(ray_end.z));

        // Compute normalized ray direction.
        Vec3 ray = ray_end - ray_start;
        //ray.normalize();

        // In which direction the voxel ids are incremented.
        double stepX = (ray.x >= 0) ? 1 : -1; // correct
        double stepY = (ray.y >= 0) ? 1 : -1; // correct
        double stepZ = (ray.z >= 0) ? 1 : -1; // correct

        // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
        double next_voxel_boundary_x = (current_voxel.x + stepX);
        double next_voxel_boundary_y = (current_voxel.y + stepY);
        double next_voxel_boundary_z = (current_voxel.z + stepZ);

        // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
        // the value of t at which the ray crosses the first vertical voxel boundary
        double tMaxX = (ray.x != 0) ? (next_voxel_boundary_x - ray_start.x) / ray.x : DBL_MAX; //
        double tMaxY = (ray.y != 0) ? (next_voxel_boundary_y - ray_start.y) / ray.y : DBL_MAX; //
        double tMaxZ = (ray.z != 0) ? (next_voxel_boundary_z - ray_start.z) / ray.z : DBL_MAX; //

        // tDeltaX, tDeltaY, tDeltaZ --
        // how far along the ray we must move for the horizontal component to equal the width of a voxel
        // the direction in which we traverse the grid
        // can only be FLT_MAX if we never go in that direction
        double tDeltaX = (ray.x != 0) ? 1.0 / ray.x * stepX : DBL_MAX;
        double tDeltaY = (ray.y != 0) ? 1.0 / ray.y * stepY : DBL_MAX;
        double tDeltaZ = (ray.z != 0) ? 1.0 / ray.z * stepZ : DBL_MAX;

        Vec3 diff(0, 0, 0);
        bool neg_ray = false;
        if (current_voxel.x != last_voxel.x && ray.x < 0) { diff.x--; neg_ray = true; }
        if (current_voxel.y != last_voxel.y && ray.y < 0) { diff.y--; neg_ray = true; }
        if (current_voxel.z != last_voxel.z && ray.z < 0) { diff.z--; neg_ray = true; }
        points.clear();

        points.push_back(current_voxel);
        if (neg_ray) {
            current_voxel += diff;
            points.push_back(current_voxel);
        }

        while (last_voxel != current_voxel) {
            if (chunkSystem.get(current_voxel.x, current_voxel.y, current_voxel.z) != BlockType::Air)
            {
                //points.push_back(current_voxel);
                Block b;
                b.type = BlockType::Cobblestone;
                //chunkSystem.set(current_voxel.x, current_voxel.y, current_voxel.z, b);
                Vec3 current_voxel;
                try
                {
                    current_voxel = points.at(points.size() - 2);
                    if (chunkSystem.get(current_voxel.x, current_voxel.y, current_voxel.z).type == BlockType::Air)
                    {
                        chunkSystem.set(current_voxel.x, current_voxel.y, current_voxel.z, b);
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
                    current_voxel.x += stepX;
                    tMaxX += tDeltaX;
                }
                else {
                    current_voxel.z += stepZ;
                    tMaxZ += tDeltaZ;
                }
            }
            else {
                if (tMaxY < tMaxZ) {
                    current_voxel.y += stepY;
                    tMaxY += tDeltaY;
                }
                else {
                    current_voxel.z += stepZ;
                    tMaxZ += tDeltaZ;
                }

            }
            points.push_back(current_voxel);
        }

        for (auto voxel : visited_voxels)
        {
            //std::cout << voxel << '\n';
        }

        //std::cout << current_voxel << ' '  << ray_start << '\n';

        //points.push_back(playerPos);
    }














    if (input.isButtonDown("attack"))
    {
        Vec3 ray_start = entityManager.entityGetComponent<Position>(player).value;
        const Quat& rotation = entityManager.entityGetComponent<Rotation>(player).value;
        const Vec3& direction = rotation * Vec3::forward;
        Vec3 ray_end = ray_start + direction * 5;;

        std::vector<Vec3> visited_voxels;

        // This id of the first/current voxel hit by the ray.
        // Using floor (round down) is actually very important,
        // the implicit int-casting will round up for negative numbers.
        Vec3 current_voxel(std::floor(ray_start.x),
            std::floor(ray_start.y),
            std::floor(ray_start.z));
        //std::cout << current_voxel << '\n';

        Vec3 last_voxel(std::floor(ray_end.x),
            std::floor(ray_end.y),
            std::floor(ray_end.z));

        // Compute normalized ray direction.
        Vec3 ray = ray_end - ray_start;
        //ray.normalize();

        // In which direction the voxel ids are incremented.
        double stepX = (ray.x >= 0) ? 1 : -1; // correct
        double stepY = (ray.y >= 0) ? 1 : -1; // correct
        double stepZ = (ray.z >= 0) ? 1 : -1; // correct

        // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
        double next_voxel_boundary_x = (current_voxel.x + stepX);
        double next_voxel_boundary_y = (current_voxel.y + stepY);
        double next_voxel_boundary_z = (current_voxel.z + stepZ);

        // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
        // the value of t at which the ray crosses the first vertical voxel boundary
        double tMaxX = (ray.x != 0) ? (next_voxel_boundary_x - ray_start.x) / ray.x : DBL_MAX; //
        double tMaxY = (ray.y != 0) ? (next_voxel_boundary_y - ray_start.y) / ray.y : DBL_MAX; //
        double tMaxZ = (ray.z != 0) ? (next_voxel_boundary_z - ray_start.z) / ray.z : DBL_MAX; //

        // tDeltaX, tDeltaY, tDeltaZ --
        // how far along the ray we must move for the horizontal component to equal the width of a voxel
        // the direction in which we traverse the grid
        // can only be FLT_MAX if we never go in that direction
        double tDeltaX = (ray.x != 0) ? 1.0 / ray.x * stepX : DBL_MAX;
        double tDeltaY = (ray.y != 0) ? 1.0 / ray.y * stepY : DBL_MAX;
        double tDeltaZ = (ray.z != 0) ? 1.0 / ray.z * stepZ : DBL_MAX;

        Vec3 diff(0, 0, 0);
        bool neg_ray = false;
        if (current_voxel.x != last_voxel.x && ray.x < 0) { diff.x--; neg_ray = true; }
        if (current_voxel.y != last_voxel.y && ray.y < 0) { diff.y--; neg_ray = true; }
        if (current_voxel.z != last_voxel.z && ray.z < 0) { diff.z--; neg_ray = true; }
        points.clear();

        points.push_back(current_voxel);
        if (neg_ray) {
            current_voxel += diff;
            points.push_back(current_voxel);
        }

        while (last_voxel != current_voxel) {
            if (chunkSystem.get(current_voxel.x, current_voxel.y, current_voxel.z) != BlockType::Air)
            {
                //points.push_back(current_voxel);
                Block b;
                b.type = BlockType::Air;
                chunkSystem.set(current_voxel.x, current_voxel.y, current_voxel.z, b);
                break;
            }

            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    current_voxel.x += stepX;
                    tMaxX += tDeltaX;
                }
                else {
                    current_voxel.z += stepZ;
                    tMaxZ += tDeltaZ;
                }
            }
            else {
                if (tMaxY < tMaxZ) {
                    current_voxel.y += stepY;
                    tMaxY += tDeltaY;
                }
                else {
                    current_voxel.z += stepZ;
                    tMaxZ += tDeltaZ;
                }

            }
            points.push_back(current_voxel);
        }

        for (auto voxel : visited_voxels)
        {
            //std::cout << voxel << '\n';
        }

        //std::cout << current_voxel << ' '  << ray_start << '\n';

        //points.push_back(playerPos);
    }

    //for (const auto& point : points)
    //{
    //    Debug::drawCube(Vec3(point) + Vec3(0.5, 0.5, 0.5));
    //}

    auto end = std::chrono::high_resolution_clock::now();
    glfwSetWindowTitle(engine.window().handle(), (std::string("frame time: ") + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count())).c_str());
    //std::cout << "frame time" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << '\n';
}
