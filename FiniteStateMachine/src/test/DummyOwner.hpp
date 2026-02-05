#pragma once
#include "math/vec2.hpp"
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
};

struct DummyOwner {
	Vec2 pos;
	Vec2 playerPos;
	float hp;
	Stats stats;
	MemoireIa memoireIa;
	std::string debugStateName;

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

	bool attackReady() const {
		return memoireIa.attackReady;
	};

	void setDebugStateName(const std::string& name) {
		debugStateName = name;
	}
	
	void moveToward(const Vec2& target, float dt) {
		Vec2 dir = (target - pos).normalized();
		pos = pos + dir * stats.vitesse * dt;
	}

	void requestAttack() {
		memoireIa.didAttackThisFrame = true;
		memoireIa.attackReady = false;
	}

	void resetFrameFlags() {
		memoireIa.didAttackThisFrame = false;
	}
};

