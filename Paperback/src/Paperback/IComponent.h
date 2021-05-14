#pragma once

#include "PaperbackPCH.h"
#include "Paperback/Core.h"
#include "Paperback/Log.h"

//#include <rttr/rttr_enable.h> // RTTR_ENABLE(...)
//#include <rttr/registration_friend.h> // RTTR_REGISTRATION_FRIEND

namespace Paperback
{
	static const ComponentType MAX_UNIQUE_COMPONENTS = 32;
	using Signature = std::bitset<MAX_UNIQUE_COMPONENTS>;


	class PAPERBACK_API IComponent
	{
	public:

		friend class Entity;
		friend class ISystem;

		IComponent() {}
		virtual ~IComponent() = default;
		virtual void Init();

		//RTTR_ENABLE();

	private:

		//RTTR_REGISTRATION_FRIEND
	};
}