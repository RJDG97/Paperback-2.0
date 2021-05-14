#pragma once

#include "Paperback/Core.h"
#include "Paperback/IComponent.h"

namespace Paperback
{
	using EntityList = std::set<EntityID>;

	class PAPERBACK_API ISystem
	{
	public:

		friend class IApplication;
		friend class SystemManager;

		virtual void Init() {}
		virtual void Update(float frametime, [[maybe_unused]] bool paused = false) = 0;

		void RemoveEntity(EntityID id);
		void RegisterEntity(EntityID id);
		void SetSignature(Signature sig);
		virtual std::string Name() = 0;

		template <ComponentType T>
		void Register()
		{
			requirements.set( static_cast<size_t>(T) );
		}

		template <ComponentType T>
		void Remove()
		{
			requirements.set( static_cast<size_t>(T), false );
		}

		virtual ~ISystem() = default;

	protected:

	#pragma warning(disable : 4251)
		Signature requirements;
		EntityList list;
	#pragma warning(default : 4251)
	};
}