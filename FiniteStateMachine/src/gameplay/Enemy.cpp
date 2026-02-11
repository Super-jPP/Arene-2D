#include "Enemy.h"

#include <cstdint>

static Vec2 normalizeSafe(const Vec2& v)
{
    const float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len <= 0.00001f) return Vec2{0.f, 0.f};
    return Vec2{ v.x / len, v.y / len };
}

Enemy::Enemy()
: Enemy(Vec2{200.f, 200.f}, 120.f, 450.f, 60.f, 0.8f, EnemyKind::Blue)
{
}

Enemy::Enemy(const Vec2& pos, float speed, float detectRadius, float attackRange, float attackCooldown, EnemyKind kind)
{
    m_pos = pos;
    m_kind = kind;

    // Init per-enemy RNG with a simple hash of position + kind.
    // (Deterministic but "random enough" for gameplay.)
    const std::uint32_t px = static_cast<std::uint32_t>(std::fabs(pos.x) * 1000.f);
    const std::uint32_t py = static_cast<std::uint32_t>(std::fabs(pos.y) * 1000.f);
    const std::uint32_t k  = (m_kind == EnemyKind::Green) ? 0x9E3779B9u : 0x7F4A7C15u;
    m_rngState = (px * 2654435761u) ^ (py * 2246822519u) ^ k;
    if (m_rngState == 0u) m_rngState = 0xA341316Cu;

    m_stats.vitesse = speed;
    m_stats.detectRadius = detectRadius;
    m_stats.lostRadius = detectRadius * 1.4f; // simple default
    m_stats.attackRange = attackRange;

    m_attackCooldownTimer = attackCooldown;

    m_shape.setRadius(14.f);
    m_shape.setOrigin({14.f, 14.f});
    m_shape.setPosition({ m_pos.x, m_pos.y });

    // Visual tweak per enemy kind
    if (m_kind == EnemyKind::Green)
    {
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(sf::Color(20, 120, 20));
    }

    // Start in Idle for a couple seconds, then it will go back to Wander.
    m_fsm.setInitial(EnnemyIdle<Enemy>::instance(), *this);
    applyStateColor();
}

float Enemy::rand01()
{
    // xorshift32
    std::uint32_t x = m_rngState;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    m_rngState = x;

    // 24-bit mantissa-ish normalization -> [0,1)
    return static_cast<float>(x & 0x00FFFFFFu) / 16777216.f;
}

void Enemy::update(float dt, const Vec2& playerPos)
{
    m_playerPos = playerPos;

    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= dt;

    m_fsm.update(*this, dt);

    m_shape.setPosition({ m_pos.x, m_pos.y });

    // Visual tweak per enemy kind
    if (m_kind == EnemyKind::Green)
    {
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(sf::Color(20, 120, 20));
    }
    applyStateColor();
}

void Enemy::draw(sf::RenderTarget& rt) const
{
    rt.draw(m_shape);
}

void Enemy::moveDir(Vec2 dir, float dt)
{
    dir = normalizeSafe(dir);
    m_pos.x += dir.x * m_stats.vitesse * dt;
    m_pos.y += dir.y * m_stats.vitesse * dt;
}

void Enemy::moveToward(Vec2 target, float dt)
{
    const Vec2 d = Vec2{ target.x - m_pos.x, target.y - m_pos.y };
    moveDir(d, dt);
}

void Enemy::changeState(EnemyStateId id)
{
    switch (id)
    {
        case EnemyStateId::Wander: m_fsm.changeState(EnemyWander<Enemy>::instance(), *this); break;
        case EnemyStateId::Chase:  m_fsm.changeState(EnemyChase<Enemy>::instance(),  *this); break;
        case EnemyStateId::Attack: m_fsm.changeState(EnemyAttack<Enemy>::instance(), *this); break;
        case EnemyStateId::Dead:   m_fsm.changeState(EnemyDead<Enemy>::instance(),   *this); break;
        case EnemyStateId::Idle:   m_fsm.changeState(EnnemyIdle<Enemy>::instance(), *this); break;
        default: break;
    }
}

void Enemy::takeDamage(int amount) {
    if (isDead()) return;

    m_hp -= amount;

    // Petit effet visuel (clignote rouge)
    m_shape.setFillColor(sf::Color::Red);

    if (m_hp <= 0) {
        // Déclenche l'état mort de la FSM
        m_fsm.changeState(EnemyDead<Enemy>::instance(), *this);
    }
}

int Enemy::extractPendingDamage()
{
    int dmg = m_pendingDamageToPlayer;
    m_pendingDamageToPlayer = 0;
    return dmg;
}

// Update this existing method
void Enemy::triggerAttackHit(float cooldown)
{
    m_attackCooldownTimer = cooldown;

    // Check distance to player to validate the hit
    float dist = Vec2::distance(m_pos, m_playerPos);

    // If player is within attack range + small buffer, apply damage
    if (dist <= m_stats.attackRange * 1.2f)
    {
        m_pendingDamageToPlayer = 10; // Value could be in AIStats
    }
}

void Enemy::applyStateColor()
{
    const std::string n = m_fsm.getStateName();

    // Two enemy "types" so the CDC requirements can be validated without sprites:
    // - Blue: classic palette
    // - Green: alternate palette
    if (m_kind == EnemyKind::Green)
    {
        if (n == "Wander")       m_shape.setFillColor(sf::Color(80, 220, 120));
        else if (n == "Chase")   m_shape.setFillColor(sf::Color(40, 190, 90));
        else if (n == "Attack")  m_shape.setFillColor(sf::Color(20, 150, 70));
        else if (n == "Dead")    m_shape.setFillColor(sf::Color(40, 40, 40));
        else                     m_shape.setFillColor(sf::Color(140, 210, 160));
        return;
    }

    // Default (Blue) palette
    if (n == "Wander")       m_shape.setFillColor(sf::Color(80, 140, 255));
    else if (n == "Chase")   m_shape.setFillColor(sf::Color(255, 160, 60));
    else if (n == "Attack")  m_shape.setFillColor(sf::Color(255, 70, 70));
    else if (n == "Dead")    m_shape.setFillColor(sf::Color(40, 40, 40));
    else                     m_shape.setFillColor(sf::Color(180, 180, 180));
}
