#pragma once

#include "PaperbackPCH.h"

namespace Sandbox
{
	// Creating local events Engine Interface
	// Currently used in Pause System
	struct RandomEvent : public Paperback::IEvent
	{
		RandomEvent(Paperback::EntityID _id) : id{ _id } {}
		Paperback::EntityID id;
	};

	struct CustomEvent : public Paperback::IEvent
	{
		CustomEvent(Paperback::EntityID _id) : id{ _id } {}
		Paperback::EntityID id;
	};
}