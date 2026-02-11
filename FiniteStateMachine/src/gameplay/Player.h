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

    // --- Collision "pieds" (top-down)
    // Petite hitbox en bas du sprite : empêche le joueur de marcher sur les obstacles
    // uniquement avec les pieds (plus naturel visuellement).
    Vec2 feetPosition() const;
    float feetRadius() const;

    void setPosition(const Vec2& p);
    float collisionRadius() const;


    // Clamp player inside the world (map) bounds
    void setWorldBounds(const Vec2& worldSize);

private:
    Vec2 m_position;
    Vec2 m_direction;
    Vec2 m_worldSize{ 0.f, 0.f }; // world/map size in pixels
    float m_speed = 0.f;

    // Collider "pieds" (en pixels monde). Ajustable selon le sprite.
    // Top-down "feet" collider: small circle near the bottom of the sprite.
    // This avoids the common issue where the head/upper body collides with floor obstacles.
    float m_feetRadius = 12.f;
    float m_feetInsetFromBottom = 14.f; // distance from sprite bottom to the collider center

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
