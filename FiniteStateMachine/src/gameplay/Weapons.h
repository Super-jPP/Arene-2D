#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "math/Vec2.h"
#include "Enemy.h"

class Weapons
{
public:
    Weapons();

    // Basic sword swing (kept for compatibility)
    void swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies);

    // Explicit hitbox (used for multi-hit slashes: frame 2 + frame 7)
    // - range: radius of the hit circle
    // - forwardOffset: how far in front of the player the hit circle center is placed
    void swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies, float range, float forwardOffset);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:
    float m_range;      // Portée de l'épée
    float m_damage;     // Dégâts

    // Pour le debug visuel de la zone d'attaque
    sf::CircleShape m_debugHitzone;
    float m_debugTimer;
};
