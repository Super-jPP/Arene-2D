#include "Spawner.h"
#include <algorithm>

void Spawner::init(const Vec2& worldSize)
{
    m_worldSize = worldSize;
    m_enemies.clear();
    m_spawnAcc = 0.f;

    // Preset VS-like
    m_spawnRate   = 3.0f;
    m_maxEnemies  = 250;
    m_spawnMargin = 140.f;
}

float Spawner::randf(float a, float b)
{
    std::uniform_real_distribution<float> d(a, b);
    return d(m_rng);
}

Vec2 Spawner::randomOutsideView(const sf::View& view, const Vec2& playerPos)
{
    // IMPORTANT:
    // If the caller accidentally passes a view that is NOT following the player,
    // enemies will spawn far away and you won't see them.
    // So we use a robust heuristic: if the view center is "far" from the player,
    // we spawn around the player instead.
    sf::Vector2f center = view.getCenter();
    const sf::Vector2f size = view.getSize();

    const float dx = center.x - playerPos.x;
    const float dy = center.y - playerPos.y;

    // If center is more than ~1 screen away from the player, fallback to playerPos.
    const float farThreshold = (size.x + size.y) * 0.5f;
    if ((dx * dx + dy * dy) > (farThreshold * farThreshold))
        center = sf::Vector2f(playerPos.x, playerPos.y);

    const float halfW = size.x * 0.5f;
    const float halfH = size.y * 0.5f;

    // Spawn just outside the screen, but not too far (VS-like).
    // Tune these if needed:
    const float minExtra = 20.f;   // minimum distance outside the view
    const float maxExtra = 120.f;  // maximum distance outside the view

    const int side = static_cast<int>(randf(0.f, 4.f));
    sf::Vector2f p;

    if (side == 0) { // TOP
        p.x = center.x + randf(-halfW, halfW);
        p.y = center.y - halfH - randf(minExtra, maxExtra);
    }
    else if (side == 1) { // BOTTOM
        p.x = center.x + randf(-halfW, halfW);
        p.y = center.y + halfH + randf(minExtra, maxExtra);
    }
    else if (side == 2) { // LEFT
        p.x = center.x - halfW - randf(minExtra, maxExtra);
        p.y = center.y + randf(-halfH, halfH);
    }
    else { // RIGHT
        p.x = center.x + halfW + randf(minExtra, maxExtra);
        p.y = center.y + randf(-halfH, halfH);
    }

    // Keep inside world bounds, but avoid "teleporting" to edges when the player is near borders.
    // We clamp softly by shrinking the allowed area by a small padding.
    const float pad = 8.f;
    p.x = std::clamp(p.x, pad, std::max(pad, m_worldSize.x - pad));
    p.y = std::clamp(p.y, pad, std::max(pad, m_worldSize.y - pad));

    return Vec2{ p.x, p.y };
}

void Spawner::update(float dt, const Vec2& playerPos, const sf::View& cameraView)
{
    // Spawn timer
    m_spawnAcc += dt * m_spawnRate;

    while (m_spawnAcc >= 1.f && m_enemies.size() < m_maxEnemies)
    {
        m_spawnAcc -= 1.f;

        const Vec2 pos = randomOutsideView(cameraView, playerPos);

        // Stats (VS-like)
        const float speed = randf(80.f, 130.f);

        // Detect radius: keep it reasonable so enemies actually spend time in Idle/Wander
        // before noticing the player (FSM states become observable).
        const float detect = 450.f;
        const float range  = 60.f;
        const float cd     = randf(0.6f, 1.0f);

                const EnemyKind kind = (randf(0.f, 1.f) < 0.5f) ? EnemyKind::Blue : EnemyKind::Green;

        m_enemies.emplace_back(pos, speed, detect, range, cd, kind);
    }

    for (Enemy& e : m_enemies)
        e.update(dt, playerPos);
}



void Spawner::draw(sf::RenderTarget& rt)
{
    for (const Enemy& e : m_enemies)
        e.draw(rt);
}
