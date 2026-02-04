#pragma once

#include <iostream>
#include <ostream>

#include "NpcContext.h"
#include "State.h"

namespace NpcAi
{
    class PatrolState : public FSM::State<NpcContext>
    {
        void Enter(NpcContext _context) override
        {
            std::cout << "Enter Patrol _State" << std::endl;
        }

        void Exit(NpcContext _context) override
        {
            std::cout << "Exit Patrol _State" << std::endl;
        }
    };
}
