#pragma once

#include "ai/fsm/IState.hpp"
#include "ai/AttackTypes.hpp"

#include <string>

template <typename Owner>
class EnemyChase;

struct AttackDef
{
    const char* animBase;
    float windup;
    float active;
    float recovery;
    float cooldown;
};

inline AttackDef getAttackDef(AttackType t)
{
    switch (t)
    {
    case AttackType::Heavy: return { "AttackHeavy", 0.18f, 0.06f, 0.25f, 0.9f };
    case AttackType::Dash:  return { "AttackDash",  0.12f, 0.05f, 0.20f, 0.8f };
    case AttackType::Light:
    default:                return { "AttackLight", 0.10f, 0.05f, 0.18f, 0.6f };
    }
}

inline const char* phaseSuffix(AttackPhase p)
{
    switch (p)
    {
    case AttackPhase::Windup:   return "_Windup";
    case AttackPhase::Active:   return "_Active";
    case AttackPhase::Recovery: return "_Recovery";
    default:                    return "";
    }
}

template <typename Owner>
class EnemyAttack : public ai::fsm::IState<Owner>
{
public:
    static EnemyAttack& instance()
    {
        static EnemyAttack s;
        return s;
    }

    const char* name() const override { return "Attack"; }

    void onEnter(Owner& owner) override
    {
        owner.setDebugStateName("Attack");

        owner.setAttackPhase(AttackPhase::Windup);
        owner.setAttackPhaseTimer(getAttackDef(owner.getAttackType()).windup);
        owner.setHitDone(false);

        // Animation state (neutre, le renderer fera le mapping)
        const AttackDef def = getAttackDef(owner.getAttackType());
        owner.setAnimState(std::string(def.animBase) + phaseSuffix(owner.getAttackPhase()));
    }

    void onUpdate(Owner& owner, float dt) override
    {
        const AttackDef def = getAttackDef(owner.getAttackType());

        owner.decAttackPhaseTimer(dt);

        // Phase switching
        if (owner.getAttackPhase() == AttackPhase::Windup)
        {
            if (owner.getAttackPhaseTimer() <= 0.f)
            {
                owner.setAttackPhase(AttackPhase::Active);
                owner.setAttackPhaseTimer(def.active);
                owner.setAnimState(std::string(def.animBase) + phaseSuffix(owner.getAttackPhase()));
            }
        }
        else if (owner.getAttackPhase() == AttackPhase::Active)
        {
            // Hit window: declenche UNE FOIS
            if (!owner.isHitDone())
            {
                owner.triggerAttackHit(def.cooldown);
                owner.setHitDone(true);
            }

            if (owner.getAttackPhaseTimer() <= 0.f)
            {
                owner.setAttackPhase(AttackPhase::Recovery);
                owner.setAttackPhaseTimer(def.recovery);
                owner.setAnimState(std::string(def.animBase) + phaseSuffix(owner.getAttackPhase()));
            }
        }
        else // Recovery
        {
            if (owner.getAttackPhaseTimer() <= 0.f)
            {
                // Fin d'attaque => retour en chase
                owner.changeState(EnemyChase<Owner>::instance());
                return;
            }
        }
    }

    void onExit(Owner&) override {}
};
