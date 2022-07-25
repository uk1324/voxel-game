#include <Game/GameScene.hpp>
#include <Engine/Engine.hpp>
#include <Game/Inventory/Inventory.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Components/AnimatedModel.hpp>
#include <Game/Debug/Debug.hpp>

#include <chrono>
#include <iostream>
#include <imgui.h>

GameScene::GameScene(Engine& engine)
    : Scene(engine, 100)
    , m_renderingSystem(*this)
    , m_playerMovementSystem(*this)
    , m_physicsSystem(*this)
    , m_inventorySystem(*this)
    , m_inventory(9 * 4)
    , m_isGamePaused(false)
{
    Debug::init(input, m_renderingSystem);
    //engine.window().setPos(Vec2I(600, 400));
    engine.window().hideCursor();
    registerInputActions();

    entityManager.registerComponent<Position>();
    entityManager.registerComponent<Rotation>();
    entityManager.registerComponent<AnimatedModel>();
    entityManager.registerComponent<ItemComponent>();
    entityManager.registerComponent<ZombieComponent>();

    m_player = entityManager.createEntity();
    entityManager.addComponent(m_player, Position{ Vec3(0, 80, 0) });
    //entityManager.entityEmplaceComponent<Position>(m_player, Vec3(-31, -7, -237));
    entityManager.addComponent(m_player, Rotation{ Quat::identity });
    entityManager.addComponent(m_player, PlayerMovementComponent{});

    PhysicsAabbCollider collider;
    collider.centerOffset = Vec3(0, -(1.62 - 0.5 * (1.875)), 0);
    collider.halfSize = Vec3(0.6, 1.875, 0.6) / 2.0f;
    entityManager.addComponent(m_player, collider);
    entityManager.addComponent(m_player, PhysicsVelocity{ Vec3(0, 0, 0) });
    entityManager.addComponent(m_player, Grounded{ false });

    input.registerKeyboardButton("testtest", KeyCode::T);
}

// Maybe make an unloaded component and down update unloaded entites like player and remove unloaded entites that don't need to exist.
void GameScene::update()
{
    auto start = std::chrono::high_resolution_clock::now();

    Debug::update(input);

    if (input.isButtonDown("exit") && (ImGui::GetIO().WantCaptureKeyboard == false))
    {
        engine.stop();
    }

    if (input.isButtonDown("pause"))
    {
        if (m_inventorySystem.isInventoryOpen)
            m_inventorySystem.isInventoryOpen = false;
        else
        {
            if (m_isGamePaused)
                engine.window().hideCursor();
            else
                engine.window().showCursor();
            m_isGamePaused = !m_isGamePaused;
        }
    }

    const Vec3 playerPos = entityManager.getComponent<Position>(m_player).value;
    const Quat playerRot = entityManager.getComponent<Rotation>(m_player).value;
    const Vec2 windowSize = Vec2(engine.window().getWindowSize());
    m_renderingSystem.update(
        windowSize,
        playerPos, playerRot,
        entityManager,
        m_blockSystem.chunkSystem,
        itemData
    );
    m_inventorySystem.render(m_inventory, itemData, m_blockSystem.blockData, Vec2(windowSize));

    m_console.update();
    Debug::shouldDisplayDeveloperConsole = true;
    if (Debug::shouldDisplayDeveloperConsole && m_isGamePaused)
    {
        m_console.draw();
    }

    m_blockSystem.chunkSystem.m_worldGen.updateDebugConfig();

    m_blockSystem.chunkSystem.update(entityManager.getComponent<Position>(m_player).value);


    if ((m_isGamePaused == false) && (m_inventorySystem.isInventoryOpen == false))
    {
        m_playerMovementSystem.update(m_player, input, time, entityManager);
        m_playerInteractionSystem.update(m_player, itemData, m_inventorySystem.heldItem(m_inventory), input, entityManager, m_blockSystem, m_inventory);
        if (input.isButtonDown("testtest") || time.currentTick() == 1)
        {
            m_entitySystem.spawnZombie(playerPos, entityManager);
        }
    }
    if (m_isGamePaused == false)
    {
        Opt<ItemStack> droppedItem = m_inventorySystem.update(m_inventory, engine.window(), input, itemData);
    }

    m_entitySystem.update(entityManager, input, m_blockSystem);
    m_physicsSystem.update(time, entityManager, m_blockSystem.chunkSystem);
     
    auto end = std::chrono::high_resolution_clock::now();
    std::string title = std::string("frame time: ") + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    engine.window().setTitle(title.c_str());
}

void GameScene::registerInputActions()
{
    input.registerKeyboardButton("exit", KeyCode::X);
    input.registerKeyboardButton("pause", KeyCode::Escape);

    input.registerMouseButton("attack", MouseButton::Left);
    input.registerMouseButton("use", MouseButton::Right);

    input.registerKeyboardButton("dropItem", KeyCode::Q);

    input.registerKeyboardButton("forward", KeyCode::W);
    input.registerKeyboardButton("back", KeyCode::S);
    input.registerKeyboardButton("left", KeyCode::A);
    input.registerKeyboardButton("right", KeyCode::D);
    input.registerKeyboardButton("jump", KeyCode::Space);
    input.registerKeyboardButton("crouch", KeyCode::LeftShift);

    input.registerKeyboardButton("inventoryOpen", KeyCode::E);
    input.registerKeyboardButton("hotbar0", KeyCode::Alpha1);
    input.registerKeyboardButton("hotbar1", KeyCode::Alpha2);
    input.registerKeyboardButton("hotbar2", KeyCode::Alpha3);
    input.registerKeyboardButton("hotbar3", KeyCode::Alpha4);
    input.registerKeyboardButton("hotbar4", KeyCode::Alpha5);
    input.registerKeyboardButton("hotbar5", KeyCode::Alpha6);
    input.registerKeyboardButton("hotbar6", KeyCode::Alpha7);
    input.registerKeyboardButton("hotbar7", KeyCode::Alpha8);
    input.registerKeyboardButton("hotbar8", KeyCode::Alpha9);

    input.registerKeyboardButton("test", KeyCode::F);
}
