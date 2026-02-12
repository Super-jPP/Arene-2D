#include "gameplay/Player.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace
{
    // Paths are relative to the working directory (typically the .exe folder)
    constexpr const char* kIdleSheet  = "assets/player/idle_sheet.png";
    constexpr const char* kRunSheet   = "assets/player/run_sheet.png";
    constexpr const char* kTurnSheet  = "assets/player/run_turn_sheet.png"; // optional
    constexpr const char* kSlashSheet = "assets/player/slash_sheet.png";
}

Player::Player()
    : m_position(640.f, 360.f)
    , m_direction(0.f, 0.f)
    , m_speed(250.f)
    , m_pendingFacingLeft(false)
{
    m_texIdle.loadFromFile(kIdleSheet);
    m_texRun.loadFromFile(kRunSheet);
    m_texTurn.loadFromFile(kTurnSheet);
    m_texSlash.loadFromFile(kSlashSheet);

    // --- Player animations (80x80)
    m_anim.addRowClip("Idle", m_texIdle, 80, 80, 18, 12.f, true);
    m_anim.addRowClip("Run",  m_texRun,  80, 80, 24, 25.f, true);
    // TurnAround is optional: we keep the clip registered, but we do NOT drive it anymore (VS-like)
    m_anim.addRowClip("TurnAround", m_texTurn, 80, 80, 5, 25.f, false);

    m_anim.play("Idle", true);

    // Player sprite
    m_anim.setOrigin(40.f, 40.f);
    m_anim.setScale(3.f, 3.f);
    m_anim.setPosition(m_position.x, m_position.y);

    // --- Slash FX (128x128, 9 frames)
    {
        render::AnimationClip clip;
        clip.texture = &m_texSlash;
        clip.fps = 30.f;
        clip.loop = false;

        constexpr int fw = 128;
        constexpr int fh = 128;
        constexpr int totalFrames = 9;

        const auto size = m_texSlash.getSize();
        const int cols = static_cast<int>(size.x) / fw;
        const int rows = static_cast<int>(size.y) / fh;

        clip.frames.reserve(totalFrames);

        int count = 0;
        for (int y = 0; y < rows && count < totalFrames; ++y)
        {
            for (int x = 0; x < cols && count < totalFrames; ++x)
            {
                clip.frames.emplace_back(sf::IntRect({ x * fw, y * fh }, { fw, fh }));
                ++count;
            }
        }

        m_slashAnim.addClip("Slash", clip);
    }

    // Slash FX sprite (centered)
    m_slashAnim.setOrigin(64.f, 64.f);
    m_slashAnim.setScale(3.f, 3.f);
    m_slashAnim.setPosition(m_position.x, m_position.y);
}

bool Player::consumeAttackTrigger()
{
    const bool v = m_attackTrigger;
    m_attackTrigger = false;
    return v;
}

bool Player::consumeAttackHit1()
{
    const bool v = m_hit1Pending;
    m_hit1Pending = false;
    return v;
}

bool Player::consumeAttackHit2()
{
    const bool v = m_hit2Pending;
    m_hit2Pending = false;
    return v;
}

void Player::setWorldBounds(const Vec2& worldSize)
{
    m_worldSize = worldSize;
}

void Player::update(float dt, const Vec2& moveDir, bool aimLeft)
{
    auto isZero2 = [](const Vec2& v)
    {
        return (std::abs(v.x) < 0.01f) && (std::abs(v.y) < 0.01f);
    };

    // Facing is mouse-driven (VS-like)
    m_pendingFacingLeft = aimLeft;
    m_anim.setFacingLeft(aimLeft);

    // --- Auto-attack cooldown
    if (m_attackCooldownRemaining > 0.f)
        m_attackCooldownRemaining = std::max(0.f, m_attackCooldownRemaining - dt);

    // --- Attack start (automatic)
    if (!m_isAttacking && m_attackCooldownRemaining <= 0.f)
    {
        m_isAttacking = true;
        m_attackTrigger = true;
        m_attackCooldownRemaining = m_attackCooldown;

        // Start slash FX (do NOT replace player anim)
        m_slashPlaying = true;
        m_slashFacingLeft = aimLeft; // freeze direction during the swing
        m_slashAnim.setFacingLeft(m_slashFacingLeft);
        m_slashAnim.play("Slash", true);

        // Reset hit timings (frame 2 and 7 @30fps)
        m_slashElapsed = 0.f;
        m_hit1Pending = false;
        m_hit2Pending = false;
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

    // --- Player locomotion (player stays visible)
    if (isZero2(moveDir))
        m_anim.play("Idle");
    else
        m_anim.play("Run");

    m_anim.setPosition(m_position.x, m_position.y);
    m_anim.update(dt);

    // --- Slash FX update
    m_slashAnim.setPosition(m_position.x, m_position.y);

    if (m_slashPlaying)
    {
        m_slashElapsed += dt;

        // Hit #1 (frame 2)
        if (!m_hit1Pending && m_slashElapsed >= m_hit1Time)
            m_hit1Pending = true;

        // Hit #2 (frame 7)
        if (!m_hit2Pending && m_slashElapsed >= m_hit2Time)
            m_hit2Pending = true;

        m_slashAnim.update(dt);

        if (m_slashAnim.isFinished())
        {
            m_slashPlaying = false;
            m_isAttacking = false;

            // Safety: do not leave pending hits after animation
            m_hit1Pending = false;
            m_hit2Pending = false;
        }
    }
    else
    {
        // Still tick to keep internal time consistent (no visible effect)
        m_slashAnim.update(dt);
    }
}

void Player::takeDamage(int amount)
{
    m_hp -= amount;
    std::printf("Aie ! PV restants : %d\n", m_hp);
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_anim.sprite());
    if (m_slashPlaying)
        window.draw(m_slashAnim.sprite());
}

Vec2 Player::position() const
{
    return m_position;
}

Vec2 Player::feetPosition() const
{
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
