#pragma once

#include "ai/fsm/IState.hpp"
#include "math/vec2.hpp"



template <typename Owner>
class EnemyChase;

template <typename Owner>
class EnemyWander : public ai::fsm::IState<Owner>
{
public:
    const char* name() const override
    {
        return "Wander";
    }

    void onEnter(Owner& owner) override
    {
        owner.setDebugStateName("Wander");

        if (owner.memoireIa.wanderTimer <= 0.f)
            owner.memoireIa.wanderTimer = 1.0f;

        if (owner.memoireIa.wanderDir.x == 0.f &&
            owner.memoireIa.wanderDir.y == 0.f)
        {
            owner.memoireIa.wanderDir = Vec2{ 1.f, 0.f };
        }
    }

    void onUpdate(Owner& owner, float dt) override
    {
        const float dist = Vec2::distance(owner.getPos(), owner.getPlayerPos());
        if (dist <= owner.getStats().detectRadius)
        {
            owner.changeState(EnemyChase<Owner>::instance());
            return;
        }

        owner.memoireIa.wanderTimer -= dt;

        if (owner.memoireIa.wanderTimer <= 0.f)
        {
            Vec2& d = owner.memoireIa.wanderDir;

            if (d.x == 1.f && d.y == 0.f)       d = Vec2{ 0.f, 1.f };
            else if (d.x == 0.f && d.y == 1.f)  d = Vec2{ -1.f, 0.f };
            else if (d.x == -1.f && d.y == 0.f) d = Vec2{ 0.f, -1.f };
            else                                d = Vec2{ 1.f, 0.f };

            owner.memoireIa.wanderTimer = 1.0f;
        }

        owner.moveDir(owner.memoireIa.wanderDir, dt);
    }

    void onExit(Owner&) override
    {
        
    }

    static EnemyWander& instance()
    {
        static EnemyWander s;
        return s;
    }

};
