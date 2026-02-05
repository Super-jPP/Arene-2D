#pragma once
#include "math/vec2.hpp"
#include <string>

class DummyOwner {
public: 
	float moveSpeed = 0;
	float detectRadius = 0;
	float lostRadius = 0;
	float attackRange = 0;

	int pos;
	int playerPos;
		float hp;
		std::string debugStateName;
		
	void wanderDir();
	float wanderTimer();
	float spawnTimer();
	bool attackReady();
	bool didAttackThisFrame();

};