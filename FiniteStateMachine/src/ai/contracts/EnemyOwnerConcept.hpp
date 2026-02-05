#pragma once

#include <concepts>
#include <utility> // std::declval
#include <string>

#include "math/Vec2.hpp"
#include "ai/AttackTypes.hpp"
#include "ai/types/AIStats.hpp"
#include "ai/fsm/IState.hpp"

namespace ai::contracts
{
    template <typename Owner>
    concept EnemyOwner =
        requires(Owner o, const Owner co,
                 float dt, const Vec2 v,
                 const char* cstr,
                 const std::string& s,
                 AttackType at, AttackPhase ap)
        {
            { co.getPos() } -> std::same_as<Vec2>;
            { co.getPlayerPos() } -> std::same_as<Vec2>;
            { co.getStats() } -> std::same_as<const AIStats&>;

            { o.moveDir(v, dt) } -> std::same_as<void>;
            { o.moveToward(v, dt) } -> std::same_as<void>;

            { o.changeState(std::declval<ai::fsm::IState<Owner>&>()) } -> std::same_as<void>;

            { o.setDebugStateName(s) } -> std::same_as<void>;
            { o.setAnimState(s) } -> std::same_as<void>;

            { co.getWanderDir() } -> std::same_as<Vec2>;
            { o.setWanderDir(v) } -> std::same_as<void>;
            { co.getWanderTimer() } -> std::same_as<float>;
            { o.setWanderTimer(dt) } -> std::same_as<void>;
            { o.decWanderTimer(dt) } -> std::same_as<void>;

            { co.attackReady() } -> std::same_as<bool>;
            { co.getAttackType() } -> std::same_as<AttackType>;
            { o.setNextAttackType(at) } -> std::same_as<void>;

            { co.getAttackPhase() } -> std::same_as<AttackPhase>;
            { o.setAttackPhase(ap) } -> std::same_as<void>;
            { co.getAttackPhaseTimer() } -> std::same_as<float>;
            { o.setAttackPhaseTimer(dt) } -> std::same_as<void>;
            { o.decAttackPhaseTimer(dt) } -> std::same_as<void>;

            { co.isHitDone() } -> std::same_as<bool>;
            { o.setHitDone(true) } -> std::same_as<void>;

            { o.triggerAttackHit(dt) } -> std::same_as<void>;
        };
}
