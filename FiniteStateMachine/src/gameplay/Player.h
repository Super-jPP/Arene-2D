#pragma once

#include <SFML/Graphics.hpp>
#include "math/Vec2.h"

#include "render/Animator.h"

class Player
{
public:
	Player();

	void update(float dt, const Vec2& moveDir, bool wantsAttack);
	void draw(sf::RenderWindow& window) const;

	Vec2 position() const;

private:
	Vec2 m_position;
	Vec2 m_direction;
	float m_speed;

	// --- Animation ---
	render::Animator m_anim;

	sf::Texture m_texIdle;
	sf::Texture m_texRun;
	sf::Texture m_texLightAttack;

	bool m_isAttacking = false;
};