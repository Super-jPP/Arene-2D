#include <iostream>

#include "ai/fsm/FSM.hpp"
#include "ai/states/EnemyWander.hpp"
#include "ai/states/EnemyChase.hpp"
#include "test/DummyOwner.hpp"
#include "math/Vec2.hpp"

int main()
{
    DummyOwner dummy;


    dummy.pos = Vec2{ 0.f, 0.f };
    dummy.playerPos = Vec2{ 300.f, 0.f };   
    dummy.hp = 100.f;

    dummy.stats.vitesse = 50.f;
    dummy.stats.detectRadius = 100.f;
    dummy.stats.lostRadius = 140.f;      
    dummy.stats.attackRange = 20.f;

    dummy.memoireIa.wanderDir = Vec2{ 1.f, 0.f };
    dummy.memoireIa.wanderTimer = 1.0f;
    dummy.memoireIa.spawnTimer = 0.f;
    dummy.memoireIa.attackReady = true;
    dummy.memoireIa.didAttackThisFrame = false;

    // --- FSM ---
    ai::fsm::FSM<DummyOwner> fsm;
    dummy.bindFSM(fsm);

    fsm.setInitial(EnemyWander<DummyOwner>::instance(), dummy);

    const float dt = 1.f / 60.f;
    const int totalFrames = 6 * 60; 

    for (int i = 0; i < totalFrames; ++i)
    {
        const float t = i * dt;

        dummy.resetFrameFlags();

   
        if (t < 2.f)
            dummy.playerPos = Vec2{ 300.f, 0.f };
        else if (t < 4.f)
            dummy.playerPos = Vec2{ 50.f, 0.f };
        else
            dummy.playerPos = Vec2{ 300.f, 0.f };

        fsm.update(dummy, dt);


        if (i % 10 == 0)
        {
            const float dist = Vec2::distance(dummy.getPos(), dummy.getPlayerPos());

            std::cout
                << "t=" << t
                << "  state=" << fsm.getStateName()
                << "  debug=" << dummy.debugStateName
                << "  pos=(" << dummy.pos.x << "," << dummy.pos.y << ")"
                << "  player=(" << dummy.playerPos.x << "," << dummy.playerPos.y << ")"
                << "  dist=" << dist
                << "\n";
        }
    }

    return 0;
}
