#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Rotation.hpp>
#include <Engine/Graphics/Drawing.hpp>

#include <Engine/Debug/Drawing.hpp>

#include <iostream>

#include <math.h>

GameScene::GameScene(Engine& engine)
	: Scene(engine, 100)
    , shader("src/Game/Blocks/chunk.vert", "src/Game/Blocks/chunk.frag")
    , textureArray(16, 16, { "assets/textures/blocks/dirt.png", "assets/textures/blocks/stone.png", "assets/textures/blocks/cobblestone.png", "assets/textures/blocks/grass.png", "assets/textures/blocks/grass2.png" })
    , cameraSystem(90, 0.1f, 1000.0f)
    , skyboxShader("src/Game/cubemap.vert", "src/Game/cubemap.frag")
    , skybox( Gfx::CubeMapTexturePaths { "assets/textures/skybox/right.png", "assets/textures/skybox/left.png", "assets/textures/skybox/top.png", "assets/textures/skybox/bottom.png", "assets/textures/skybox/front.png", "assets/textures/skybox/back.png" })
    , vbo(point, sizeof(point))
    , pointShader("src/Game/b.vert", "src/Game/a.frag")
{
    vao.bind();
    vbo.bind();
    vao.setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    input.registerKeyboardButton("exit", KeyCode::Escape);
    input.registerKeyboardButton("test", KeyCode::F);
    input.registerKeyboardButton("test1", KeyCode::E);
    input.registerMouseButton("attack", MouseButton::Left);
    input.registerMouseButton("use", MouseButton::Right);

    Gfx::setClearColor(Color(0.52f, 0.80f, 0.92f, 1.0f));

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10);
}

void GameScene::update()
{
    if (input.isButtonDown("exit"))
    {
        engine.stop();
    }

    Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);

    playerMovementSystem.update(*this, player);

    //entityManager.entityGetComponent<Rotation>(player).value = Quat::identity;
    
    Vec2I windowSize = engine.window().getWindowSize();
    glViewport(0, 0, windowSize.x, windowSize.y);
    cameraSystem.update(entityManager, player, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    chunkSystem.update(entityManager.entityGetComponent<Position>(player).value);

    shader.setMat4("model", Mat4::identity);
    shader.setMat4("camera", cameraSystem.view());
    shader.setMat4("projection", cameraSystem.projection());


    textureArray.bind();

    int i = 0;

    for (const auto& chunk : chunkSystem.m_chunksToDraw)
    {
        i++;
        shader.use();
        chunkSystem.m_vao.bind();
        Vec3I pos = chunk->pos;
        shader.setVec3I("chunkPos", pos);
         Gfx::drawTriangles(chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

        Mat4 model = Mat4::identity;
        model.scale(Vec3(8, 8, 8));
        model = model * Mat4::translation(Vec3(pos.x, pos.y, pos.z) * Chunk::SIZE + Vec3(8, 8, 8));
        // model = model * Mat4::translation(Vec3(pos.x * Chunk::SIZE - 8, pos.y * Chunk::SIZE - 8, pos.z * Chunk::SIZE - 8));

        pointShader.setMat4("model", model);
        pointShader.setMat4("view", cameraSystem.view());
        pointShader.setMat4("projection", cameraSystem.projection());
        pointShader.use();
        Gfx::Primitives::cubeTrianglesVao->bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //std::cout << chunk->pos << ' ' << chunk->vboByteOffset << ' ' << chunk->vertexCount << '\n';
    }

    //std::cout << "draw: " << i << " chunks\n";
    //glDisable(GL_CULL_FACE);

    skybox.bind();
    Gfx::Primitives::cubeTrianglesVao->bind();
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    skyboxShader.setMat4("projection", cameraSystem.projection());
    Mat4 view = cameraSystem.view();
    view.removeTranslation();
    skyboxShader.setMat4("view", view);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glFrontFace(GL_CCW);
    Gfx::drawTriangles(0, 36);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CW);
    glDepthMask(GL_TRUE);

    engine.window().hideCursor();

    vao.bind();
    pointShader.use();
    pointShader.setVec3("color", Vec3(1, 0, 0));
    for (Vec3 point : points)
    {
        pointShader.setMat4("model", Mat4::translation(point));
        pointShader.setMat4("view", cameraSystem.view());
        pointShader.setMat4("projection", cameraSystem.projection());
        
        //vao.bind();
        //glDrawArrays(GL_POINTS, 0, 1);
    }

    pointShader.setMat4("model", Mat4::identity);
    pointShader.setMat4("view", Mat4::identity);
    pointShader.setMat4("projection", Mat4::identity);
    pointShader.setVec3("color", Vec3(0, 0, 0));
    glDrawArrays(GL_POINTS, 0, 1);

    /*Gfx::Primitives::cubeTrianglesVao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);*/

    //std::cout << entityManager.entityGetComponent<Position>(player).value << '\n';

    //Box b(Vec3(4.5, 4.5, 4.5), Vec3(1, 1, 1));
    //if (b.contains(entityManager.entityGetComponent<Position>(player).value))
    //{
    //    std::cout << "Collision\n";
    //}
    if (input.isButtonDown("test"))
    {
        chunkSystem.set(1006, 29, 993, BlockType::Cobblestone);
        //chunkSystem.remesh();
    }

    if (input.isButtonDown("test1"))
    {
        chunkSystem.remesh();
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




}
