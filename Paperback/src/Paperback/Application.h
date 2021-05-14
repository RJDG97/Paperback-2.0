#pragma once

#include "PaperbackPCH.h"

#include "Core.h"

#include "Managers/SystemManager.h"
#include "Managers/EntityManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/ComponentManager.h"

#include "Events/EventBus.h"
#include "Log.h"

namespace Paperback
{
	class PAPERBACK_API Application
	{
	public:

		using SystemTimer = std::unordered_map<std::string, float>;

	public:

		Application();
		~Application();

		void Init();
		void GameLoop();

		/****************************************************/
		/*!					 Game Status
		/****************************************************/
		void Pause(bool status = false);
		bool Pause() const;

		/****************************************************/
		/*!					System Methods
		/****************************************************/
		template <typename SystemType>
		void SetSignature(Signature signature)
		{
			mSystemManager->SetSignature<SystemType>(signature);
		}

		template <typename SystemType>
		void RegisterSystem()
		{
			mSystemManager->RegisterSystem<SystemType>();
		}

		template <typename SystemType>
		SystemType* GetSystem()
		{
			return mSystemManager->GetSystem<SystemType>();
		}

		/****************************************************/
		/*!					Manager Methods
		/****************************************************/
		template <typename ManagerType>
		void RegisterManager()
		{
			mResourceManager->RegisterManager<ManagerType>();
		}

		template <typename ManagerType>
		ManagerType* GetManager()
		{
			return mResourceManager->GetManager<ManagerType>();
		}

		/****************************************************/
		/*!				 	 Event Methods
		/****************************************************/
		template <typename EventType, typename ...Args>
		void PublishEvent(Args&&... args)
		{
			mEventBus->PublishEvent<EventType>(std::forward<Args>(args)...);
		}

		template <class EventType>
		void SubscribeEvent(Action<Ref<EventType>> event)
		{
			mEventBus->SubscribeEvent<EventType>(event);
		}

		/****************************************************/
		/*!					Entity Methods
		/****************************************************/
		[[nodiscard("New EntityID Discarded")]]
		EntityID CreateEntity();
		void DeleteEntity(EntityID id);
		void DeleteAllEntities();

		/****************************************************/
		/*!				   Archetype Methods
		/****************************************************/
		void DeleteArchetype(EntityID id);
		void DeleteAllArchetypes();

		/****************************************************/
		/*!				   Component Methods
		/****************************************************/
		template <typename ComponentType>
		[[nodiscard("Component Pointer Discarded")]]
		ComponentType* GetComponent(EntityID id)
		{
			return mComponentManager->GetComponent<ComponentType>(id);
		}

		template <typename ComponentType>
		[[nodiscard("Component Type Discarded")]]
		Paperback::ComponentType GetComponentType() const
		{
			return mComponentManager->GetComponentType<ComponentType>();
		}

		template <typename ComponentType>
		void AddComponentToEntity(EntityID id, Ref<ComponentType> component)
		{
			mComponentManager->AddComponent(id, component);

			auto signature = mEntityManager->GetEntitySignature(id);
			signature.set(mComponentManager->GetComponentType<ComponentType>(), true);
			mEntityManager->SetEntitySignature(id, signature);

			mSystemManager->EntitySignatureUpdated(id, signature);
		}

		template <typename ComponentType>
		void RemoveComponentFromEntity(EntityID id)
		{
			mComponentManager->RemoveComponent<ComponentType>(id);

			auto signature = mEntityManager->GetEntitySignature(id);
			signature.set(mComponentManager->GetComponentType<ComponentType>(), false);
			mEntityManager->SetEntitySignature(id, signature);

			mSystemManager->EntitySignatureUpdated(id, signature);
		}

		/*
		// Potentially split into Entity and Archetype managers

		template <typename ComponentType>
		void AddComponentToArchetype(EntityID id, Ref<ComponentType> component)
		{
			// Archetypes are potentially just ID's with Bitsets
		}

		template <typename ComponentType>
		void RemoveComponentFromArchetype(EntityID id)
		{
			// Archetypes are potentially just ID's with Bitsets
		}
		*/

		// ADD REGISTER AND REMOVE COMPONENT (??)

	private:

	#pragma warning(disable : 4251)
		bool paused, game_active;

		// Engine Managers
		Scope<SystemManager> mSystemManager;
		Scope<EntityManager> mEntityManager;
		Scope<ResourceManager> mResourceManager;
		Scope<ComponentManager> mComponentManager;

		// Event Bus
		Ref<EventBus> mEventBus;

		//// Timer / Performance Tracker
		//Scope<Timer> mTimer;
		//SystemTimer system_timer;

		int counter{}; // Debugging tool
	#pragma warning(default : 4251)
	};

	extern PAPERBACK_API Ref<Application> app;

	// Create an Application
	static Ref<Application> CreateApplication()
	{
		app = CreateRef<Application>();
		return app;
	}
}