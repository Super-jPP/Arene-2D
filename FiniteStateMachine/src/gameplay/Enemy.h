#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <cstdint>

// ---- Fix macro collision ("None") ----
// Certains headers peuvent définir un macro None (rare mais possible).
#ifdef None
#undef None
#endif
// --------------------------------------

#include "ai/EnemyStateId.hpp"
#include "ai/AttackTypes.hpp"
#include "ai/fsm/FSM.hpp"
#include "ai/types/AIStats.hpp"

#include "ai/states/EnemyWander.hpp"
#include "ai/states/EnemyChase.hpp"
#include "ai/states/EnemyAttack.hpp"
#include "ai/states/EnemyDead.hpp"
#include "ai/states/EnnemyIdle.hpp"

#include "math/Vec2.h"
#include "render/Animator.h"

// Concrete AI-driven enemy that satisfies ai::contracts::EnemyOwnerConcept.
enum class EnemyKind
{
    Blue,
    Green,
    Bat
};

class Enemy
{
public:
    Enemy();
    Enemy(const Vec2& pos, float speed, float detectRadius, float attackRange, float attackCooldown, EnemyKind kind = EnemyKind::Blue);

    void update(float dt, const Vec2& playerPos);
    void draw(sf::RenderTarget& rt) const;
    void takeDamage(int amount);
    int getHp() const { return m_hp; }
    bool isDead() const { return m_hp <= 0; }

    // ---- EnemyOwnerConcept API ----
    Vec2 position() const { return m_pos; }
    Vec2 getPos() const { return m_pos; }
    Vec2 getPlayerPos() const { return m_playerPos; }
    const AIStats& getStats() const { return m_stats; }
    EnemyKind getKind() const { return m_kind; }

    void moveDir(Vec2 dir, float dt);
    void moveToward(Vec2 target, float dt);

    // State switching entrypoint used by AI states
    void changeState(EnemyStateId id);

    // Debug/anim hooks (optional visuals)
    void setDebugStateName(const std::string& s) { m_debugState = s; }
    void setAnimState(const std::string& s) { m_animState = s; } // placeholder for later Animator

    Vec2 getWanderDir() const { return m_wanderDir; }
    void setWanderDir(Vec2 v) { m_wanderDir = v; }
    float getWanderTimer() const { return m_wanderTimer; }
    void setWanderTimer(float t) { m_wanderTimer = t; }
    void decWanderTimer(float dt) { m_wanderTimer -= dt; }

    // Idle timer (used by Idle state; can also be triggered from Wander)
    float getIdleTimer() const { return m_idleTimer; }
    void setIdleTimer(float t) { m_idleTimer = t; }
    void decIdleTimer(float dt) { m_idleTimer -= dt; }

    // Wander -> Idle random scheduling
    float getWanderNextIdleIn() const { return m_wanderNextIdleIn; }
    void setWanderNextIdleIn(float t) { m_wanderNextIdleIn = t; }
    void decWanderNextIdleIn(float dt) { m_wanderNextIdleIn -= dt; }

    float getWanderIdleCooldown() const { return m_wanderIdleCooldown; }
    void setWanderIdleCooldown(float t) { m_wanderIdleCooldown = t; }
    void decWanderIdleCooldown(float dt) { m_wanderIdleCooldown -= dt; }

    // Small deterministic RNG per-enemy (keeps templates generic, no global srand needed)
    float rand01();
    float randRange(float a, float b) { return a + (b - a) * rand01(); }

    bool attackReady() const { return m_attackCooldownTimer <= 0.f; }
    AttackType getAttackType() const { return m_attackType; }
    void setNextAttackType(AttackType at) { m_attackType = at; }

    AttackPhase getAttackPhase() const { return m_attackPhase; }
    void setAttackPhase(AttackPhase ap) { m_attackPhase = ap; }
    float getAttackPhaseTimer() const { return m_attackPhaseTimer; }
    void setAttackPhaseTimer(float t) { m_attackPhaseTimer = t; }
    void decAttackPhaseTimer(float dt) { m_attackPhaseTimer -= dt; }

    bool isHitDone() const { return m_hitDone; }
    void setHitDone(bool v) { m_hitDone = v; }

    // Retrieve damage intended for player and reset it
    int extractPendingDamage();

    // Helper for collision
    float getRadius() const { return m_shape.getRadius(); }

    void triggerAttackHit(float cooldown);
protected: // <-- IMPORTANT : protected, pas private, pour Bones/Wolf
    Vec2 m_position; // <-- Renommé (était m_pos) pour coller à Bones.cpp

private:
    void applyStateColor();
    bool m_batRunStarted = false;

    int m_hp = 3; // Example HP
    int m_pendingDamageToPlayer = 0;

    Vec2 m_pos{};
    Vec2 m_playerPos{};

    AIStats m_stats{};
    ai::fsm::FSM<Enemy> m_fsm{};

    // Wander state data
    Vec2  m_wanderDir{ 1.f, 0.f };
    float m_wanderTimer = 0.f;

    // Idle data
    float m_idleTimer = 0.f;

    // Wander -> Idle random
    float m_wanderNextIdleIn = 0.f;
    float m_wanderIdleCooldown = 0.f;

    // RNG state
    std::uint32_t m_rngState = 0xA341316Cu;

    // Attack data
    AttackType  m_attackType = AttackType::Light;

    // ✅ AttackPhase n'a pas "None" dans AttackTypes.hpp, donc on met Windup par défaut
    AttackPhase m_attackPhase = AttackPhase::Windup;

    float m_attackPhaseTimer = 0.f;
    float m_attackCooldownTimer = 0.f;
    bool  m_hitDone = false;

    // Debug / animation placeholders
    std::string m_debugState = "None";
    std::string m_animState = "Idle";

    // Visual
    EnemyKind m_kind = EnemyKind::Blue;
    sf::CircleShape m_shape;

    // Sprite-based visuals for the bat chaser (EnemyKind::Bat)
    bool m_useSprite = false;
    render::Animator m_anim;
};
