#include "gameplay/Player.h"
#include <algorithm>
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
    , m_direction(0.f, 0.f)
    , m_speed(250.f)
{
    m_texIdle.loadFromFile(kIdleSheet);
    m_texRun.loadFromFile(kRunSheet);
    m_texLightAttack.loadFromFile(kAtkSheet);

    // NOTE: these sheets are 1-row, 80x80 frames in your current setup
    m_anim.addRowClip("Idle", m_texIdle, 80, 80, 18, 12.f, true);
    m_anim.addRowClip("Run",  m_texRun,  80, 80, 24, 16.f, true);
    m_anim.addRowClip("LightAttack", m_texLightAttack, 80, 80, 26, 20.f, false);

    m_anim.play("Idle", true);

    m_anim.setOrigin(40.f, 40.f);
    m_anim.setScale(3.f, 3.f); // upscale x3 (as requested)
    m_anim.setPosition(m_position.x, m_position.y);
}

void Player::setWorldBounds(const Vec2& worldSize)
{
    m_worldSize = worldSize;
}

void Player::update(float dt, const Vec2& moveDir, bool wantsAttack)
{
    // Attack start
    if (wantsAttack && !m_isAttacking)
    {
        m_isAttacking = true;
        m_anim.play("LightAttack", true);
    }

    // Movement
    m_position = m_position + (moveDir * m_speed * dt);

    // Clamp player inside world bounds
    if (m_worldSize.x > 0.f && m_worldSize.y > 0.f)
    {
        // Margin = simplified "radius" so the player doesn't clip outside.
        const float margin = 40.f;
        m_position.x = std::clamp(m_position.x, margin, m_worldSize.x - margin);
        m_position.y = std::clamp(m_position.y, margin, m_worldSize.y - margin);
    }

    // Facing
    if (moveDir.x < -0.01f)
        m_anim.setFacingLeft(true);
    else if (moveDir.x > 0.01f)
        m_anim.setFacingLeft(false);

    // Return to locomotion when attack ends
    if (m_isAttacking && m_anim.isFinished())
    {
        m_isAttacking = false;
        if (moveDir.isZero())
            m_anim.play("Idle", true);
        else
            m_anim.play("Run", true);
    }

    // Locomotion selection
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
