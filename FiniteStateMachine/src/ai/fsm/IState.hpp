#pragma once

namespace ai::fsm {

	template <typename Owner> 

	class IState {

	public:

		virtual ~IState() = default;

		virtual void onEnter(Owner&) = 0;
		virtual void onUpdate(Owner&, float dt) = 0;
		virtual void onExit(Owner&) = 0;

		virtual const char* name()const = 0;
	};
}