#include "gameplay/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <gameplay/Weapons.h>

namespace
{
    // Paths are relative to the working directory (typically the .exe folder)
    constexpr const char* kIdleSheet = "assets/player/idle_sheet.png";
    constexpr const char* kRunSheet  = "assets/player/run_sheet.png";
    constexpr const char* kAtkSheet  = "assets/player/light_attack_sheet.png";
}

Player::Player()
    : m_position(640.f, 360.f)
    , m_speed(250.f)
{
    // --- Load textures (fail fast if missing)
    m_texIdle.loadFromFile(kIdleSheet);
    m_texRun.loadFromFile(kRunSheet);
    m_texLightAttack.loadFromFile(kAtkSheet);

    // --- Register clips (all are 1-row sheets)
    // Each frame is 80x80 in provided sheets
    m_anim.addRowClip("Idle", m_texIdle, 80, 80, 18, 12.f, true);
    m_anim.addRowClip("Run",  m_texRun,  80, 80, 24, 16.f, true);
    // The sheet contains 26 frames (single row). We play all frames as a light combo.
    m_anim.addRowClip("LightAttack", m_texLightAttack, 80, 80, 26, 20.f, false);

    m_anim.play("Idle", true);

    // Visual setup
    m_anim.setOrigin(40.f, 40.f);
    m_anim.setScale(2.0f, 2.0f);
    m_anim.setPosition(m_position.x, m_position.y);
}

void Player::update(float dt, const Vec2& moveDir, bool wantsAttack)
{
    // Start attack only on rising edge-like behavior (continuous press is okay)
    if (wantsAttack && !m_isAttacking)
    {
        m_isAttacking = true;
        m_anim.play("LightAttack", true);
    }

    // Movement (allowed even during attack; you can lock it if you prefer)
    m_position += moveDir * m_speed * dt;

    // Facing
    if (moveDir.x < -0.01f)
        m_anim.setFacingLeft(true);
    else if (moveDir.x > 0.01f)
        m_anim.setFacingLeft(false);

    // When attack finishes, return to locomotion
    if (m_isAttacking && m_anim.isFinished())
    {
        m_isAttacking = false;
        // Force refresh based on current movement
        if (moveDir.isZero())
            m_anim.play("Idle", true);
        else
            m_anim.play("Run", true);
    }

    // Locomotion clip selection if not attacking
    if (!m_isAttacking)
    {
        if (moveDir.isZero())
            m_anim.play("Idle");
        else
            m_anim.play("Run");
    }

    m_anim.setPosition(m_position.x, m_position.y);
    m_anim.update(dt);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_anim.sprite());
}

Vec2 Player::position() const
{
    return m_position;
}