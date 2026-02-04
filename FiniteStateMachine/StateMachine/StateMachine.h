#pragma once

#include <vector>

#include "State.h"

namespace FSM
{
    template<typename ContextType>
    class StateMachine
    {
        using State = State<ContextType>;

    public:
        template<typename StateType>
        StateType* CreateState();

        void Init(State* _initialState, ContextType _context);
        void Update(ContextType _context);

    private:
        std::vector<State*> states;

        State* currentState = nullptr;
    };
}

#include "StateMachine.inl"
