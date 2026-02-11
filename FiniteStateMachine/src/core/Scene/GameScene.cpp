#include "core/Scene/GameScene.h"

#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

GameScene::GameScene()
{
    // Camera 1920x1080
    m_camera.setSize({ 1920.f, 1080.f });

    // Load map
    m_map.loadFromFile("assets/Maps/forest_clear.png", 25, 25);

    const sf::Vector2f ws = m_map.worldSize();
    m_worldSize = { ws.x, ws.y };

    // Give world bounds to player
    m_player.setWorldBounds(m_worldSize);

    // Build static obstacles (CDC requirement)
    m_obstacles.buildDefault({ m_worldSize.x, m_worldSize.y });

    // Init enemy spawner (enemies are simple debug circles for now)
    m_spawner.init(m_worldSize);
    m_spawner.setSpawnRate(4.0f);
    m_spawner.setMaxEnemies(120);

    // Camera starts on player
    m_camera.setCenter({
        m_player.position().x,
        m_player.position().y
        });
}

void GameScene::update(float dt, sf::RenderWindow& window)
{
    // --- Keyboard input (SFML direct)
    Vec2 moveDir{ 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        moveDir.x += 1.f;

    bool wantsAttack =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    // --- Update player
    m_player.update(dt, moveDir, wantsAttack);

    // --- Player vs static obstacles
    {
        // Top-down: collide with a small "feet" hitbox so only the feet can't walk on obstacles.
        const Vec2 bodyPos = m_player.position();
        const Vec2 feet0 = m_player.feetPosition();

        sf::Vector2f feet{ feet0.x, feet0.y };
        const float r = m_player.feetRadius();

        if (m_obstacles.resolveCircle(feet, r))
        {
            // Apply the correction delta back to the player's body position.
            const sf::Vector2f delta{ feet.x - feet0.x, feet.y - feet0.y };
            m_player.setPosition({ bodyPos.x + delta.x, bodyPos.y + delta.y });
        }
    }

    // --- Update enemies
    // IMPORTANT: pass the camera that follows the player so spawns are just outside the view.
    m_spawner.update(dt, m_player.position(), m_camera);

    // --- Camera clamp
    const sf::Vector2f viewSize = m_camera.getSize();
    const sf::Vector2f half = viewSize * 0.5f;

    sf::Vector2f center(
        m_player.position().x,
        m_player.position().y
    );

    center.x = std::clamp(center.x, half.x, m_worldSize.x - half.x);
    center.y = std::clamp(center.y, half.y, m_worldSize.y - half.y);

    m_camera.setCenter(center);
}

void GameScene::draw(sf::RenderWindow& window)
{
    window.setView(m_camera);

    window.draw(m_map);
    m_obstacles.draw(window);
    m_spawner.draw(window);
    m_player.draw(window);
}
