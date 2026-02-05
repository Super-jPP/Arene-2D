#include "GameScene.h"
#include "input/Input.h"

GameScene::GameScene() {
  
}

void GameScene::update(float dt, sf::RenderWindow& window) {
    Vec2 moveDir = m_input.movementDirection();
    m_player.update(dt, moveDir);

    if (m_input.isAttacking())
    {
        Vec2 mousePos = m_input.getMousePosition(window);
        Vec2 playerPos = m_player.position();

        Vec2 shootDir = mousePos - playerPos;

        if (!shootDir.isZero())
        {
            shootDir.normalize();
            m_weapons.fire(playerPos, shootDir);
        }
    }

    m_weapons.update(dt);
}

void GameScene::draw(sf::RenderWindow& window) {
    m_player.draw(window);
    m_weapons.draw(window);
}
