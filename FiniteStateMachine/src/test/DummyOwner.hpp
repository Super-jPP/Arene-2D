#pragma once
#include "math/Vec2.h"
#include "ai/AttackTypes.hpp"
#include "ai/EnemyStateId.hpp"

// State headers for ID->instance mapping
#include "ai/states/EnemyWander.hpp"
#include "ai/states/EnemyChase.hpp"
#include "ai/states/EnemyAttack.hpp"
#include "ai/states/EnemyDead.hpp"
#include "ai/states/EnnemyIdle.hpp"
#include "ai/fsm/FSM.hpp"
#include "ai/fsm/IState.hpp"

#include <string>

struct Stats {
	float vitesse;
	float detectRadius;
	float lostRadius;
	float attackRange;
};

struct MemoireIa {
	Vec2 wanderDir;
	float wanderTimer;
	float spawnTimer;

	bool attackReady;
	bool didAttackThisFrame;


	AttackType attackType = AttackType::Light;
	AttackPhase attackPhase = AttackPhase::Windup;
	float phaseTimer = 0.f;

	float attackCooldownTimer = 0.f; 
	bool hitDone = false;
};

struct DummyOwner {
	Vec2 pos;
	Vec2 playerPos;
	float hp;
	Stats stats;
	// Mémoire interne de l'IA (les states ne doivent pas y accéder directement)
	MemoireIa memoireIa;
	std::string debugStateName;
	ai::fsm::FSM<DummyOwner>* fsm = nullptr;
	std::string animState;


	Vec2 getPos() const {
		return pos;
	};

	Vec2 getPlayerPos() const {
		return playerPos;
	};
	
	float getHP() const {
		return hp;
	};

	const Stats& getStats() const {
		return stats;
	};

	void setDebugStateName(const std::string& name) {
		debugStateName = name;
	}

	// ------------------------------------------------------------
	// Interface Owner "propre" (évite tout accès direct à memoireIa)
	// ------------------------------------------------------------

	// Wander memory
	Vec2 getWanderDir() const { return memoireIa.wanderDir; }
	void setWanderDir(const Vec2& d) { memoireIa.wanderDir = d; }
	float getWanderTimer() const { return memoireIa.wanderTimer; }
	void setWanderTimer(float t) { memoireIa.wanderTimer = t; }
	void decWanderTimer(float dt) { memoireIa.wanderTimer -= dt; }

	// Attack selection / phases
	AttackType getAttackType() const { return memoireIa.attackType; }
	void setNextAttackType(AttackType t) { memoireIa.attackType = t; }
	AttackPhase getAttackPhase() const { return memoireIa.attackPhase; }
	void setAttackPhase(AttackPhase p) { memoireIa.attackPhase = p; }
	float getAttackPhaseTimer() const { return memoireIa.phaseTimer; }
	void setAttackPhaseTimer(float t) { memoireIa.phaseTimer = t; }
	void decAttackPhaseTimer(float dt) { memoireIa.phaseTimer -= dt; }
	bool isHitDone() const { return memoireIa.hitDone; }
	void setHitDone(bool v) { memoireIa.hitDone = v; }
	void setAttackCooldown(float seconds) { memoireIa.attackCooldownTimer = seconds; }
	float getAttackCooldown() const { return memoireIa.attackCooldownTimer; }
	bool didAttackThisFrame() const { return memoireIa.didAttackThisFrame; }

	// Déclenche le "hit" de l'attaque une seule fois + applique le cooldown fourni.
	void triggerAttackHit(float cooldownSeconds)
	{
		memoireIa.didAttackThisFrame = true;
		memoireIa.attackReady = false;
		memoireIa.attackCooldownTimer = cooldownSeconds;
	}
	
	void moveToward(const Vec2& target, float dt) {
		Vec2 dir = (target - pos).normalized();
		pos = pos + dir * stats.vitesse * dt;
	}

	// (legacy) si tu veux garder l'API, redirige vers triggerAttackHit
	void requestAttack() { triggerAttackHit(0.6f); }


	void resetFrameFlags() {
		memoireIa.didAttackThisFrame = false;
	}

	void moveDir(const Vec2& direction, float dt)
	{
		if (direction.x == 0.f && direction.y == 0.f)
			return;
		Vec2 dir = direction.normalized();
		pos = pos + dir * stats.vitesse * dt;
	}

	void bindFSM(ai::fsm::FSM<DummyOwner>& f)
	{
		fsm = &f;
	}

	// -----------------------------------------------------------------
	// Update IA centralisé (dead check + cooldown + update FSM)
	// NOTE: côté gameplay, Dev B fera l'équivalent dans Enemy::update().
	// -----------------------------------------------------------------
	void updateAI(float dt)
	{
		resetFrameFlags();
		updateCooldown(dt);

		// Dead check centralisé (évite de dupliquer dans chaque state)
		if (hp <= 0.f)
		{
			changeState(EnemyStateId::Dead);
			return;
		}

		if (fsm)
			fsm->update(*this, dt);
	}

	void changeState(ai::fsm::IState<DummyOwner>& next)
	{
		if (!fsm) return; 
		fsm->changeState(next, *this);
	}
// Convenience overload to avoid inter-state type dependencies in headers.
void changeState(EnemyStateId id)
{
    if (!fsm) return;
    switch (id)
    {
    case EnemyStateId::Wander: fsm->changeState(EnemyWander<DummyOwner>::instance(), *this); break;
    case EnemyStateId::Chase:  fsm->changeState(EnemyChase<DummyOwner>::instance(), *this);  break;
    case EnemyStateId::Attack: fsm->changeState(EnemyAttack<DummyOwner>::instance(), *this); break;
    case EnemyStateId::Dead:   fsm->changeState(EnemyDead<DummyOwner>::instance(), *this);   break;
    case EnemyStateId::Idle:   fsm->changeState(EnnemyIdle<DummyOwner>::instance(), *this);  break;
    }
}


	bool attackReady() const {
		return memoireIa.attackReady;
	}

	void setAnimState(const std::string& s) {
		animState = s;
	}

	void updateCooldown(float dt) {
		if (!memoireIa.attackReady) {
			memoireIa.attackCooldownTimer -= dt;
			if (memoireIa.attackCooldownTimer <= 0.f) {
				memoireIa.attackCooldownTimer = 0.f;
				memoireIa.attackReady = true;
			}
		}
	}

	void applyDamage(float dmg)
	{
		hp -= dmg;
		if (hp < 0.f) hp = 0.f;
	}

};

