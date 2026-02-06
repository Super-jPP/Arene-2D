#pragma once

#include "ai/fsm/IState.hpp"
#include "ai/AttackTypes.hpp"
#include "ai/EnemyStateId.hpp"
#include "math/Vec2.hpp"
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
        // facultatif: rend l'état observable visuellement
        owner.setAnimState("Run");
    }

    void onUpdate(Owner& owner, float dt) override
    {
        const float dist = Vec2::distance(owner.getPos(), owner.getPlayerPos());

        if (dist > owner.getStats().lostRadius)
        {
            owner.changeState(EnemyStateId::Wander);
            return;
        }
       
        if (owner.attackReady() && dist <= owner.getStats().attackRange)
        {
            if (dist <= owner.getStats().attackRange * 0.40f)
                owner.setNextAttackType(AttackType::Heavy);
            else if (dist <= owner.getStats().attackRange * 0.85f)
                owner.setNextAttackType(AttackType::Dash);
            else
                owner.setNextAttackType(AttackType::Light);

            owner.changeState(EnemyStateId::Attack);
            return;
        }

        owner.moveToward(owner.getPlayerPos(), dt);
    }

    void onExit(Owner&) override
    {
        
    }
};
