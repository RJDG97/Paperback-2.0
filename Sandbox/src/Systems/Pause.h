#pragma once
#ifndef PAUSE_H
#define PAUSE_H

#include <Paperback.h>

#include "PaperbackPCH.h"
//#include "Systems/ISystem.h"
#include "Events/TestEvents.h"

namespace Sandbox
{
	class Pause : public Paperback::ISystem
	{
	public:

		Pause();
		void Init() override;
		void Update(float frametime, [[maybe_unused]] bool paused = false) override;

		void TestRandomEvent(Paperback::Ref<RandomEvent> event);
		void TestCustomEvent(Paperback::Ref<CustomEvent> event);

		std::string Name() { return "Pause"; }

	private:

		bool test_remove{true};
	};
}

#endif