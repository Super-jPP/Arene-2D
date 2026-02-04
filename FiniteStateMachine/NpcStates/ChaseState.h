#pragma once
#include <iostream>

#include "NpcContext.h"
#include "StateMachine/State.h"

namespace NpcAi
{
    class ChaseState : public FSM::State<NpcContext>
    {
        void Enter(NpcContext _context) override
        {
            std::cout << "Enter Chase _State" << std::endl;
        }

        void Exit(NpcContext _context) override
        {
            std::cout << "Exit Chase _State" << std::endl;
        }
    };
}
