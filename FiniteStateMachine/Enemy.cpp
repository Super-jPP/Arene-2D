#include "gameplay/Enemy.h"

Enemy::Enemy(Vec2 pos, float speed, sf::Color color)
    : m_position(pos), m_speed(speed)
{
    m_shape.setRadius(15.f);
    m_shape.setOrigin({ 15.f, 15.f });
    m_shape.setFillColor(color);
    m_shape.setPosition({ m_position.x, m_position.y });
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(m_shape);
}