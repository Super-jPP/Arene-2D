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

    // Clamp player inside the world (map) bounds
    void setWorldBounds(const Vec2& worldSize);

private:
    Vec2 m_position;
    Vec2 m_direction;
    Vec2 m_worldSize{ 0.f, 0.f }; // world/map size in pixels
    float m_speed = 0.f;

    bool m_isAttacking = false;

    // --- Animation ---
    render::Animator m_anim;

    sf::Texture m_texIdle;
    sf::Texture m_texRun;
    sf::Texture m_texLightAttack;
    sf::Texture m_texTurn; // 🔹 turnaround animation

    float m_lastMoveX = 0.f;
    bool  m_isTurning = false;
    bool  m_pendingFacingLeft = false;
    bool  m_turnSheetFacesRight = true; // mets false si ta sheet est native "vers gauche"


};
