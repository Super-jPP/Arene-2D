#pragma once
#include <SFML/Graphics.hpp>
#include "math/Vec2.h"
#include "Player.h"

class Obstacle {
public: 
    Obstacle(Vec2 pos, Vec2 size) : m_position(pos), m_size(size) {
        m_shape.setPosition({ pos.x, pos.y });
        m_shape.setSize({ size.x, size.y });
        m_shape.setFillColor(sf::Color(100, 100, 100)); // Gris
    }

    void draw(sf::RenderWindow& window) const { window.draw(m_shape); }

    // Pour les collisions plus tard
    sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

private:
    Vec2 m_position;
    Vec2 m_size;
    sf::RectangleShape m_shape;
};