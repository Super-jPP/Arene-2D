#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Obstacles
{
public:
    void buildDefault(const sf::Vector2f& worldSize);
    void draw(sf::RenderWindow& window) const;

    // Collision: cercle (pieds) vs rectangles + boue (ellipses)
    bool resolveCircle(sf::Vector2f& pos, float radius) const;

private:
    // Blocs "durs"
    std::vector<sf::RectangleShape> m_blocks;

    // Boue (ellipses)
    struct MudPuddle
    {
        sf::CircleShape shape;  // visuel
        sf::Vector2f center;    // centre visuel
        float rx = 0.f;         // demi-axe X
        float ry = 0.f;         // demi-axe Y
    };
    std::vector<MudPuddle> m_mud;

private:
    // ✅ Déclaré ici (membre statique)
    static float randRange(float a, float b);

    static bool circleIntersectsRect(const sf::Vector2f& c, float r, const sf::FloatRect& rect);
    static sf::Vector2f pushOutCircleFromRect(const sf::Vector2f& c, float r, const sf::FloatRect& rect);
};
