#pragma once
#include "ai/fsm/IState.hpp"
#include "ai/EnemyStateId.hpp"
#include "math/Vec2.hpp"

// Minimal "Idle" state: stand still, can transition to Wander based on spawn timer if provided.
// To keep this generic, we only use methods that already exist in DummyOwner in this repo.

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
        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onUpdate(Owner& owner, float /*dt*/) override
    {
        // Default: do nothing (stay idle).
        owner.moveDir(Vec2{0.f, 0.f}, 0.f);
    }

    void onExit(Owner& /*owner*/) override {}
};
