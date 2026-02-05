#pragma once

#include "ai/fsm/IState.hpp"
#include "math/vec2.hpp"

template <typename Owner>
class EnemyWander;

template <typename Owner>
class EnemyChase : public ai::fsm::IState<Owner>
{
public: 
    static EnemyChase& instance()
    {
        static EnemyChase s;
        return s;
    }

    const char* name() const override
    {
        return "Chase";
    }

    void onEnter(Owner& owner) override
    {
        owner.setDebugStateName("Chase");
    }

    void onUpdate(Owner& owner, float dt) override
    {
        const float dist = Vec2::distance(owner.getPos(), owner.getPlayerPos());

        if (dist > owner.getStats().lostRadius)
        {
            owner.changeState(EnemyWander<Owner>::instance());
            return;
        }

        owner.moveToward(owner.getPlayerPos(), dt);
    }

    void onExit(Owner&) override
    {
        
    }
};
