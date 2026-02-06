#pragma once

#include <SFML/Graphics.hpp>
#include "math/Vec2.h"

class Player
{
public:
	Player();

	void update(float dt, const Vec2& moveDir);
	void draw(sf::RenderWindow& window) const;

	Vec2 position() const;

private:
	Vec2 m_position;
	Vec2 m_direction;
	float m_speed;

	sf::RectangleShape m_shape;
};