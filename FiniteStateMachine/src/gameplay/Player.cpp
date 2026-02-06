#include "gameplay/Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <gameplay/Weapons.h>

Player::Player()
    : m_position(640.f, 360.f)
    , m_speed(250.f)
{
    m_shape.setSize(sf::Vector2f(32.0f, 32.0f));
    m_shape.setOrigin(sf::Vector2f(16.0f, 16.0f));
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition(sf::Vector2f(m_position.x, m_position.y));
}

void Player::update(float dt, const Vec2& moveDir)
{
    m_position += moveDir * m_speed * dt;
    m_shape.setPosition(sf::Vector2f(m_position.x, m_position.y));
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

Vec2 Player::position() const
{
    return m_position;
}