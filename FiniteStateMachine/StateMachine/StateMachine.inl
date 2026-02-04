#pragma once

template<typename ContextType>
template<typename StateType>
StateType* FSM::StateMachine<ContextType>::CreateState()
{
    StateType* state = new StateType();
    states.push_back(state);

    return state;
}

template<typename ContextType>
void FSM::StateMachine<ContextType>::Init(State* _initialState, ContextType _context)
{
    currentState = _initialState;
    currentState->Enter(_context);
}

template<typename ContextType>
void FSM::StateMachine<ContextType>::Update(ContextType _context)
{
    currentState->Execute(_context);

    auto next = currentState->TryGetNext(_context);

    if (next)
    {
        currentState->Exit(_context);
        currentState = next;
        currentState->Enter(_context);
    }
}
