
#include <SFML/Graphics.hpp>
#include "core/Game.h"
#include <iostream>

#include "ai/fsm/FSM.hpp"
#include "ai/states/EnemyWander.hpp"
#include "test/DummyOwner.hpp"
#include "math/Vec2.h"


int main() {
    Game game;
    game.run();
}

static const char* toString(AttackType t)
{
    switch (t)
    {
    case AttackType::Light: return "Light";
    case AttackType::Heavy: return "Heavy";
    case AttackType::Dash:  return "Dash";
    default:                return "?";
    }
}

static const char* toString(AttackPhase p)
{
    switch (p)
    {
    case AttackPhase::Windup:   return "Windup";
    case AttackPhase::Active:   return "Active";
    case AttackPhase::Recovery: return "Recovery";
    default:                    return "?";
    }
}

//int main()
//{
//    DummyOwner dummy;
//
//
//    dummy.pos = Vec2{ 0.f, 0.f };
//    dummy.playerPos = Vec2{ 300.f, 0.f };
//    dummy.hp = 100.f;
//
//    dummy.stats.vitesse = 50.f;
//    dummy.stats.detectRadius = 100.f;
//    dummy.stats.lostRadius = 140.f;
//    dummy.stats.attackRange = 20.f;
//
//    dummy.memoireIa.wanderDir = Vec2{ 1.f, 0.f };
//    dummy.memoireIa.wanderTimer = 1.0f;
//    dummy.memoireIa.spawnTimer = 0.f;
//    dummy.memoireIa.attackReady = true;
//    dummy.memoireIa.didAttackThisFrame = false;
//
//    // --- FSM ---
//    ai::fsm::FSM<DummyOwner> fsm;
//    dummy.bindFSM(fsm);
//
//    fsm.setInitial(EnemyWander<DummyOwner>::instance(), dummy);
//
//    const float dt = 1.f / 60.f;
//    const int totalFrames = 6 * 60;
//
//    for (int i = 0; i < totalFrames; ++i)
//    {
//        const float t = i * dt;
//
//        // Scenario simple: le joueur s'approche, puis repart.
//
//        if (t < 2.f)
//            dummy.playerPos = Vec2{ 300.f, 0.f };
//        else if (t < 4.f)
//            dummy.playerPos = Vec2{ 50.f, 0.f };
//        else
//            dummy.playerPos = Vec2{ 300.f, 0.f };
//
//        // Exemple: tuer l'ennemi vers la fin pour tester EnemyDead
//        if (t > 5.0f && dummy.getHP() > 0.f)
//        {
//            dummy.applyDamage(9999.f);
//        }
//
//        dummy.updateAI(dt);
//
//
//        if (i % 10 == 0)
//        {
//            const float dist = Vec2::distance(dummy.getPos(), dummy.getPlayerPos());
//
//            std::cout
//                << "t=" << t
//                << "  state=" << fsm.getStateName()
//                << "  debug=" << dummy.debugStateName
//                << "  hp=" << dummy.getHP()
//                << "  pos=(" << dummy.pos.x << "," << dummy.pos.y << ")"
//                << "  player=(" << dummy.playerPos.x << "," << dummy.playerPos.y << ")"
//                << "  dist=" << dist
//                << "  atkReady=" << (dummy.attackReady() ? 1 : 0)
//                << "  atkType=" << toString(dummy.getAttackType())
//                << "  phase=" << toString(dummy.getAttackPhase())
//                << "  phaseT=" << dummy.getAttackPhaseTimer()
//                << "  cd=" << dummy.getAttackCooldown()
//                << "  hitFrame=" << (dummy.didAttackThisFrame() ? 1 : 0)
//                << "\n";
//        }
//    }
//
//    return 0;
//}
