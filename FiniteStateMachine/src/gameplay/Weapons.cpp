#include "gameplay/Weapons.h"

#include <cmath>

Weapons::Weapons()
    : m_range(60.f)
    , m_damage(1.f)
    , m_debugTimer(0.f)
{
    // Setup du cercle de debug (optionnel)
    m_debugHitzone.setRadius(m_range);
    m_debugHitzone.setOrigin({ m_range, m_range });
    m_debugHitzone.setFillColor(sf::Color(255, 0, 0, 50));
}

void Weapons::swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies)
{
    swingSword(playerPos, facingLeft, enemies, m_range, 40.f);
}

void Weapons::swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies, float range, float forwardOffset)
{
    // Debug circle
    m_debugTimer = 0.10f;
    m_debugHitzone.setRadius(range);
    m_debugHitzone.setOrigin({ range, range });

    const float dir = facingLeft ? -1.f : 1.f;
    const Vec2 center{ playerPos.x + dir * forwardOffset, playerPos.y };
    m_debugHitzone.setPosition({ center.x, center.y });

    for (auto& enemy : enemies)
    {
        if (enemy.isDead()) continue;

        const Vec2 d = enemy.getPos() - center;
        const float distSq = d.x * d.x + d.y * d.y;

        if (distSq <= range * range)
        {
            // Damage once per swing call (GameScene controls 2-phase timing)
            enemy.takeDamage(static_cast<int>(m_damage));
        }
    }
}

void Weapons::update(float dt)
{
    if (m_debugTimer > 0.f) m_debugTimer -= dt;
}

void Weapons::draw(sf::RenderWindow& window)
{
    if (m_debugTimer > 0.f)
        window.draw(m_debugHitzone);
}
