#pragma once

#include "ai/fsm/IState.hpp"
#include "math/Vec2.hpp"
#include "ai/EnemyStateId.hpp"
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

        // facultatif: rend l'état observable visuellement
        owner.setAnimState("Walk");

        if (owner.getWanderTimer() <= 0.f)
            owner.setWanderTimer(1.0f);

        const Vec2 d = owner.getWanderDir();
        if (d.x == 0.f && d.y == 0.f)
        {
            owner.setWanderDir(Vec2{ 1.f, 0.f });
        }
    }

    void onUpdate(Owner& owner, float dt) override
    {
        const float dist = Vec2::distance(owner.getPos(), owner.getPlayerPos());
        if (dist <= owner.getStats().detectRadius)
        {
            owner.changeState(EnemyStateId::Chase);
            return;
        }

        owner.decWanderTimer(dt);

        if (owner.getWanderTimer() <= 0.f)
        {
            Vec2 d = owner.getWanderDir();

            if (d.x == 1.f && d.y == 0.f)       d = Vec2{ 0.f, 1.f };
            else if (d.x == 0.f && d.y == 1.f)  d = Vec2{ -1.f, 0.f };
            else if (d.x == -1.f && d.y == 0.f) d = Vec2{ 0.f, -1.f };
            else                                d = Vec2{ 1.f, 0.f };

            owner.setWanderDir(d);
            owner.setWanderTimer(1.0f);
        }

        owner.moveDir(owner.getWanderDir(), dt);
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
