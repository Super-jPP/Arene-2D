#include "Enemy.h"

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

    // Start in Wander by default (but if detectRadius is huge, it will instantly switch to Chase).
    m_fsm.setInitial(EnemyWander<Enemy>::instance(), *this);
    applyStateColor();
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

void Enemy::triggerAttackHit(float cooldown)
{
    // Here you would check distance + apply damage to player.
    // For now we only start the cooldown so AI can't instantly re-attack.
    m_attackCooldownTimer = cooldown;
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
