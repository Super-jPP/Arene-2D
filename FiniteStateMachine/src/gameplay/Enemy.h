#pragma once
#include <SFML/Graphics.hpp>
#include "math/Vec2.h"

class Enemy {
public:
    Enemy(Vec2 pos, float speed, sf::Color color);
    virtual ~Enemy() = default; 

    // Méthode virtuelle : chaque enfant aura sa propre logique
    virtual void update(float dt, const Vec2& playerPos) = 0;

    void draw(sf::RenderWindow& window) const;
    Vec2 position() const { return m_position; }

protected:
    Vec2 m_position;
    float m_speed;
    sf::CircleShape m_shape;
};