#include "Obstacles.h"
#include <algorithm>
#include <cmath>
#include <random>

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// ✅ Défini en tant que membre statique de Obstacles
float Obstacles::randRange(float a, float b)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng);
}

void Obstacles::buildDefault(const sf::Vector2f& worldSize)
{
    m_blocks.clear();
    m_mud.clear();

    auto makeBlock = [&](sf::Vector2f pos, sf::Vector2f size)
        {
            sf::RectangleShape r;
            r.setPosition(pos);
            r.setSize(size);
            r.setFillColor(sf::Color(60, 60, 60));
            r.setOutlineThickness(2.f);
            r.setOutlineColor(sf::Color(30, 30, 30));
            m_blocks.push_back(r);
        };

    // Obstacles statiques "durs"
    makeBlock({ worldSize.x * 0.45f, worldSize.y * 0.45f }, { 220.f, 40.f });
    makeBlock({ worldSize.x * 0.20f, worldSize.y * 0.20f }, { 120.f, 120.f });
    makeBlock({ worldSize.x * 0.32f, worldSize.y * 0.18f }, { 80.f, 180.f });
    makeBlock({ worldSize.x * 0.70f, worldSize.y * 0.70f }, { 160.f, 90.f });
    makeBlock({ worldSize.x * 0.55f, worldSize.y * 0.25f }, { 40.f, 240.f });

    // Flaques random
    const int mudCount = 1000;
    const float margin = 120.f;

    for (int i = 0; i < mudCount; ++i)
    {
        MudPuddle p;
        p.center = {
            Obstacles::randRange(margin, worldSize.x - margin),
            Obstacles::randRange(margin, worldSize.y - margin)
        };

        p.rx = Obstacles::randRange(90.f, 140.f);
        p.ry = Obstacles::randRange(55.f, 85.f);

        // Visuel : "tache" (ellipse) SANS contour visible
        p.shape = sf::CircleShape(1.f);
        p.shape.setOrigin({ 1.f, 1.f });
        p.shape.setPosition(p.center);
        p.shape.setScale({ p.rx, p.ry });
        p.shape.setFillColor(sf::Color(110, 75, 20, 140));

        // ✅ IMPORTANT : supprime les ellipses autour (outline)
        p.shape.setOutlineThickness(0.f);
        // p.shape.setOutlineColor(...); // inutile si thickness = 0

        m_mud.push_back(p);
    }
}

void Obstacles::draw(sf::RenderWindow& window) const
{
    for (const auto& p : m_mud)
        window.draw(p.shape);

    for (const auto& b : m_blocks)
        window.draw(b);
}

bool Obstacles::circleIntersectsRect(const sf::Vector2f& c, float r, const sf::FloatRect& rect)
{
    // SFML 3 : rect.position / rect.size
    const float left = rect.position.x;
    const float top = rect.position.y;
    const float right = rect.position.x + rect.size.x;
    const float bottom = rect.position.y + rect.size.y;

    const float closestX = clampf(c.x, left, right);
    const float closestY = clampf(c.y, top, bottom);

    const float dx = c.x - closestX;
    const float dy = c.y - closestY;
    return (dx * dx + dy * dy) < (r * r);
}

sf::Vector2f Obstacles::pushOutCircleFromRect(const sf::Vector2f& c, float r, const sf::FloatRect& rect)
{
    const float left = rect.position.x;
    const float top = rect.position.y;
    const float right = rect.position.x + rect.size.x;
    const float bottom = rect.position.y + rect.size.y;

    const float closestX = clampf(c.x, left, right);
    const float closestY = clampf(c.y, top, bottom);

    sf::Vector2f p{ closestX, closestY };
    sf::Vector2f d{ c.x - p.x, c.y - p.y };

    float len2 = d.x * d.x + d.y * d.y;

    if (len2 < 0.0001f)
    {
        const float leftPen = std::abs(c.x - left);
        const float rightPen = std::abs(right - c.x);
        const float topPen = std::abs(c.y - top);
        const float bottomPen = std::abs(bottom - c.y);

        const float minPen = std::min({ leftPen, rightPen, topPen, bottomPen });

        sf::Vector2f out = c;
        if (minPen == leftPen)        out.x = left - r;
        else if (minPen == rightPen)  out.x = right + r;
        else if (minPen == topPen)    out.y = top - r;
        else                          out.y = bottom + r;

        return out;
    }

    const float len = std::sqrt(len2);
    sf::Vector2f n{ d.x / len, d.y / len };
    return sf::Vector2f{ p.x + n.x * r, p.y + n.y * r };
}

bool Obstacles::resolveCircle(sf::Vector2f& pos, float radius) const
{
    bool collided = false;

    // Rectangles
    for (const auto& b : m_blocks)
    {
        const auto rect = b.getGlobalBounds();
        if (circleIntersectsRect(pos, radius, rect))
        {
            pos = pushOutCircleFromRect(pos, radius, rect);
            collided = true;
        }
    }

    // Boue (ellipse) avec centre de collision décalé vers le bas
    const float mudColliderOffsetY = 50.f; // <<< descend la hitbox

    for (const auto& p : m_mud)
    {
        const float ellipseShrink = 0.92f;

        const float ex = (p.rx * ellipseShrink) + radius;
        const float ey = (p.ry * ellipseShrink) + radius;

        sf::Vector2f colliderCenter = p.center;
        colliderCenter.y += mudColliderOffsetY;

        sf::Vector2f d{ pos.x - colliderCenter.x, pos.y - colliderCenter.y };

        if (std::abs(d.x) < 0.0001f && std::abs(d.y) < 0.0001f)
            d.y = 1.f;

        const float nx = d.x / ex;
        const float ny = d.y / ey;
        const float v = (nx * nx) + (ny * ny);

        if (v < 1.f)
        {
            const float invLen = 1.f / std::sqrt(v);
            pos = sf::Vector2f{
                colliderCenter.x + d.x * invLen,
                colliderCenter.y + d.y * invLen
            };
            collided = true;
        }
    }

    return collided;
}
