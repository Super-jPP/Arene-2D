#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "math/Vec2.h"
#include "Enemy.h"

class Weapons {
public:
    Weapons();
    // Crée un ou plusieurs projectiles selon l'arme
    void swingSword(const Vec2& playerPos, bool facingLeft, std::vector<Enemy>& enemies);

    void update(float dt);
    void draw(sf::RenderWindow& window);

private:

    float m_range;      // Portée de l'épée
    float m_damage;     // Dégâts
    float m_knockback;  // Force de recul

    // Pour le debug visuel de la zone d'attaque
    sf::CircleShape m_debugHitzone;
    float m_debugTimer;
};