#pragma once
#include <cmath>
#include "ai/fsm/IState.hpp"
#include "ai/EnemyStateId.hpp"
#include "math/Vec2.h"

// Idle state:
// - Always stands still.
// - Has a timer so it can be used both:
//   * on spawn (a longer idle)
//   * as a short random "micro-idle" while wandering (timer set by Wander state)

template <typename Owner>
class EnnemyIdle : public ai::fsm::IState<Owner>
{
public:
    static EnnemyIdle& instance()
    {
        static EnnemyIdle s;
        return s;
    }

    const char* name() const override { return "Idle"; }

    void onEnter(Owner& owner) override
    {
        owner.setDebugStateName("Idle");
        owner.setAnimState("Idle");

        // If Idle duration hasn't been specified (spawn), pick a longer one.
        if (owner.getIdleTimer() <= 0.f)
        {
            owner.setIdleTimer(owner.randRange(1.5f, 3.0f)); // ~1.5s .. 3.0s
        }

        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onUpdate(Owner& owner, float dt) override
    {
        // If the player is already close enough, skip straight to Chase.
        const float d2 = Vec2::distanceSquared(owner.getPos(), owner.getPlayerPos());
        const float detect = owner.getStats().detectRadius;
        if (d2 <= detect * detect)
        {
            owner.changeState(EnemyStateId::Chase);
            return;
        }

        // Otherwise, wait a bit then transition to Wander.
        owner.decIdleTimer(dt);
        if (owner.getIdleTimer() <= 0.f)
        {
            owner.setIdleTimer(0.f);
            owner.changeState(EnemyStateId::Wander);
            return;
        }

        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onExit(Owner& /*owner*/) override {}
};
