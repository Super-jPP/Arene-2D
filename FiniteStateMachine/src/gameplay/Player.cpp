#include "gameplay/Player.h"
#include <algorithm>
#include <gameplay/Weapons.h>

namespace
{
    // Paths are relative to the working directory (typically the .exe folder)
    constexpr const char* kIdleSheet = "assets/player/idle_sheet.png";
    constexpr const char* kRunSheet  = "assets/player/run_sheet.png";
    constexpr const char* kAtkSheet  = "assets/player/light_attack_sheet.png";
    constexpr const char* kTurnSheet = "assets/player/run_turn_sheet.png";

}

Player::Player()
    : m_position(640.f, 360.f)
    , m_direction(0.f, 0.f)
    , m_speed(250.f)
{
    m_texIdle.loadFromFile(kIdleSheet);
    m_texRun.loadFromFile(kRunSheet);
    m_texLightAttack.loadFromFile(kAtkSheet);
    m_texTurn.loadFromFile(kTurnSheet);


    // NOTE: these sheets are 1-row, 80x80 frames in your current setup
    m_anim.addRowClip("Idle", m_texIdle, 80, 80, 18, 12.f, true);
    m_anim.addRowClip("Run",  m_texRun,  80, 80, 24, 25.f, true);
    m_anim.addRowClip("LightAttack", m_texLightAttack, 80, 80, 26, 20.f, false);
    m_anim.addRowClip("TurnAround", m_texTurn, 80, 80, 5, 25.f, false);

    m_anim.play("Idle", true);

    m_anim.setOrigin(40.f, 40.f); // centre X, bas Y

    m_anim.setScale(3.f, 3.f); // upscale x3 (as requested)
    m_anim.setPosition(m_position.x, m_position.y);
}

void Player::setWorldBounds(const Vec2& worldSize)
{
    m_worldSize = worldSize;
}

void Player::update(float dt, const Vec2& moveDir, bool wantsAttack)
{
    // --- Helpers (Vec2 may not have isZero)
    auto isZero2 = [](const Vec2& v)
        {
            return (std::abs(v.x) < 0.01f) && (std::abs(v.y) < 0.01f);
        };

    // --- Attack start
    if (wantsAttack && !m_isAttacking)
    {
        m_isAttacking = true;
        m_anim.play("LightAttack", true);
    }

    // --- Movement
    m_position = m_position + (moveDir * m_speed * dt);

    // --- Clamp player inside world bounds
    if (m_worldSize.x > 0.f && m_worldSize.y > 0.f)
    {
        const float margin = 40.f;
        m_position.x = std::clamp(m_position.x, margin, m_worldSize.x - margin);
        m_position.y = std::clamp(m_position.y, margin, m_worldSize.y - margin);
    }

    // --- Input X handling (tap-friendly)
    float moveX = moveDir.x;
    const bool hasX = (std::abs(moveX) > 0.01f);

    // Memorize last non-zero X (helps with quick taps)
    if (hasX)
        m_lastMoveX = moveX;

    const bool desiredLeft = (moveX < -0.01f);
    const bool desiredRight = (moveX > 0.01f);

    // --- Turnaround detection (no need for currentName())
    // Trigger when player is moving and wants to face the opposite direction.
    if (!m_isAttacking && !m_isTurning && hasX)
    {
        const bool currentlyLeft = m_anim.facingLeft(); // needs getter
        const bool wantsOpposite = (desiredLeft != currentlyLeft);

        if (wantsOpposite)
        {
            m_isTurning = true;
            m_pendingFacingLeft = desiredLeft;

            // IMPORTANT: during the turn animation, we show the animation in the TARGET direction
            // (prevents "inverted" look)
            m_anim.setFacingLeft(m_pendingFacingLeft);

            // Play turnaround (one-shot)
            m_anim.play("TurnAround", true);
        }
    }

    // --- If turning: hold animation until finished, then resume locomotion
    if (m_isTurning)
    {
        if (m_anim.isFinished())
        {
            m_isTurning = false;

            // Facing already set during the turn, keep it
            // Resume locomotion
            if (hasX || (!isZero2(moveDir)))
                m_anim.play("Run", true);
            else
                m_anim.play("Idle", true);
        }

        m_anim.setPosition(m_position.x, m_position.y);
        m_anim.update(dt);
        return;
    }

    // --- Normal facing (only when not turning)
    if (desiredLeft)
        m_anim.setFacingLeft(true);
    else if (desiredRight)
        m_anim.setFacingLeft(false);

    // --- Return to locomotion when attack ends
    if (m_isAttacking && m_anim.isFinished())
    {
        m_isAttacking = false;
        if (isZero2(moveDir))
            m_anim.play("Idle", true);
        else
            m_anim.play("Run", true);
    }

    // --- Locomotion selection (if not attacking)
    if (!m_isAttacking)
    {
        if (isZero2(moveDir))
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

Vec2 Player::feetPosition() const
{
    // In this project the Animator sprite origin is (0,0), so m_position is the sprite's top-left.
    // Compute feet collider from the actual sprite bounds so it stays correct even if you swap sprites.
    const sf::FloatRect gb = m_anim.sprite().getGlobalBounds();

    const float centerX = gb.position.x + gb.size.x * 0.5f;
    const float feetY   = gb.position.y + gb.size.y - m_feetInsetFromBottom;

    return { centerX, feetY };
}

float Player::feetRadius() const
{
    return m_feetRadius;
}


void Player::setPosition(const Vec2& p)
{
    m_position = p;
}

float Player::collisionRadius() const
{
    return 18.f;
}
