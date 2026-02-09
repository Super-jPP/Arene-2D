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
    m_player.draw(window);
}
