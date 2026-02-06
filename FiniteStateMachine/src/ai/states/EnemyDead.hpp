#pragma once
#include "ai/fsm/IState.hpp"
#include "ai/EnemyStateId.hpp"
#include "math/Vec2.hpp"

// Minimal "Dead" state: stop movement and stay dead.
// Generic: relies only on Owner interface methods used here.

template <typename Owner>
class EnemyDead : public ai::fsm::IState<Owner>
{
public:
    static EnemyDead& instance()
    {
        static EnemyDead s;
        return s;
    }

    const char* name() const override { return "Dead"; }

    void onEnter(Owner& owner) override
    {
        owner.setDebugStateName("Dead");
        owner.setAnimState("Dead");
        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onUpdate(Owner& owner, float /*dt*/) override
    {
        // Remain in Dead
        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onExit(Owner& /*owner*/) override {}
};
