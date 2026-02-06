#include "GameScene.h"
#include "input/Input.h"

GameScene::GameScene() {
    m_weapons.setWeaponType(WeaponType::PISTOL);
}

void GameScene::update(float dt, sf::RenderWindow& window) {
    Vec2 moveDir = m_input.movementDirection();

    m_player.update(dt, moveDir);
    m_world.update(dt, m_player.position());

    auto newWeapon = m_input.getSelectedWeapon();
    if (newWeapon.has_value()) {
        m_weapons.setWeaponType(newWeapon.value()); // Change les stats (vitesse, taille, nombre)
    }

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
    m_world.draw(window);
    m_player.draw(window);
    m_weapons.draw(window);
}
