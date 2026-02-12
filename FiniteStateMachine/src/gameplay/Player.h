#pragma once

#include <SFML/Graphics.hpp>
#include "math/Vec2.h"
#include "render/Animator.h"

class Player
{
public:
    Player();

    // Vampire-Survivors-like controls:
    // - movement: keyboard
    // - attacks: automatic (cooldown)
    // - facing/attack direction: mouse (aimLeft)
    void update(float dt, const Vec2& moveDir, bool aimLeft);
    void draw(sf::RenderWindow& window) const;

    Vec2 position() const;

    // --- Collision "pieds" (top-down)
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

    // True once when an automatic attack starts (consume to avoid repeating).
    bool consumeAttackTrigger();

    // True once for the 2 hit moments during the same swing (frame 2 and frame 7).
    bool consumeAttackHit1();
    bool consumeAttackHit2();

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

    // Auto-attack cadence
    float m_attackCooldown = 0.55f;         // seconds between attacks
    float m_attackCooldownRemaining = 0.f;  // timer interne
    bool  m_attackTrigger = false;          // déclenchement unique au start

    // --- Animation joueur ---
    render::Animator m_anim;
    sf::Texture m_texIdle;
    sf::Texture m_texRun;
    sf::Texture m_texTurn; // optionnel (pas utilisé si tu veux un VS-like pur)

    // --- FX Slash (séparé du joueur) ---
    render::Animator m_slashAnim;
    sf::Texture m_texSlash;
    bool m_slashPlaying = false;
    bool m_slashFacingLeft = false; // figé au début du swing

    // ✅ 2 hits pendant l'attaque (frame 2 et 7 sur 9 frames @30fps)
    float m_slashElapsed = 0.f;
    bool  m_hit1Pending = false;
    bool  m_hit2Pending = false;
    float m_hit1Time = 2.f / 30.f; // ~0.0667s
    float m_hit2Time = 7.f / 30.f; // ~0.2333s

    // Facing (piloté par la souris)
    bool  m_pendingFacingLeft = false;

    // --- Feet collider ---
    float m_feetRadius = 12.f;
    float m_feetInsetFromBottom = 14.f;
};
