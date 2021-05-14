#pragma once

#include "Paperback/Core.h"

namespace Paperback
{
	class PAPERBACK_API IManager
	{
	public:
		virtual void Init() {}
		// virtual void Deserialize();
		virtual ~IManager() = default;
	};
}