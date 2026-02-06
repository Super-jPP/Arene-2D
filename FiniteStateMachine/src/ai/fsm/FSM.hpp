#pragma once
#include "IState.hpp"

namespace ai::fsm {
	template <typename Owner> 
	class FSM {

	public:

		void setInitial(IState<Owner>& state, Owner& owner) {
			m_current = &state;  
			m_current->onEnter(owner);
		}

		void update(Owner& owner, float dt) {
			if (m_current) m_current->onUpdate(owner, dt);
		}

		const char* getStateName() const  {
				return m_current ? m_current->name() : "None";
			
		}

		void changeState(IState<Owner>& state, Owner& owner) {
			if (m_current == &state) {
				return;
			}

			if (m_current) {
				m_current->onExit(owner);
			}

			m_current = &state;
			m_current->onEnter(owner);

		}

	private: 

		IState<Owner>* m_current = nullptr;
	};
}