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

    void takeDamage(int amount);
    bool isDead() const { return m_hp <= 0; }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }

    bool isAttacking() const { return m_isAttacking; }
    bool isFacingLeft() const { return m_pendingFacingLeft; }

private:
    // --- Stats ---
    int m_hp = 100;
    int m_maxHp = 100;
    float m_invincibilityTimer = 0.f;

    // --- Physique ---
    Vec2 m_position;
    Vec2 m_direction;
    Vec2 m_worldSize{ 0.f, 0.f };
    float m_speed = 0.f;

    // --- États ---
    bool m_isAttacking = false;

    // --- Animation ---
    render::Animator m_anim;

    sf::Texture m_texIdle;
    sf::Texture m_texRun;
    sf::Texture m_texLightAttack;
    sf::Texture m_texTurn;

    // Variables pour le demi-tour (TurnAround)
    float m_lastMoveX = 0.f;
    bool  m_isTurning = false;
    bool  m_pendingFacingLeft = false; // <--- UNE SEULE FOIS
    bool  m_turnSheetFacesRight = true;

    float m_feetRadius = 12.f;
    float m_feetInsetFromBottom = 14.f;
};
