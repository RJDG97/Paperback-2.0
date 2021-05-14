#include "PaperbackPCH.h"
#include "EntityManager.h"

namespace Paperback
{
	EntityManager::EntityManager() : 
		entity_delete_queue{ },
		entity_count{ }, available_entities{ }, entity_signatures{ },
		archetype_count{ }, available_archetypes{ }, archetype_signatures{ }
	{
		for (EntityID id = 0; id < MAX_ENTITIES; ++id)
		{
			available_entities.push(id);
		}
		for (EntityID id = 0; id < MAX_ARCHETYPES; ++id)
		{
			available_archetypes.push(id);
		}
	}

	EntityManager::~EntityManager()
	{
		
	}

	void EntityManager::Init()
	{
		
	}

	EntityID EntityManager::CreateEntity()
	{
		//PPB_ASSERT_MSG(entity_count >= MAX_ENTITIES, 
		//			 "EntityManager::CreateEntity() -> Exceeded max entities allowed");

		EntityID id = available_entities.front();
		available_entities.pop();
		++entity_count;

		return id;
	}

	EntityID EntityManager::CloneEntity(EntityID id)
	{
		// Probably have to make additional changes to clone / make use of signatures

		//// Assert id < entity count so valid
		//
		//EntityID id = CreateEntity();
		//Signature to_clone = GetEntitySignature(id);
		
		return {};
	}

	EntityID EntityManager::CreateArchetype()
	{
		//PPB_ASSERT_MSG(archetype_count >= MAX_ARCHETYPES,
		//			 "EntityManager::CreateArchetype() -> Exceeded max archetypes allowed");

		EntityID id = available_archetypes.front();
		available_archetypes.pop();
		++archetype_count;

		return id;
	}

	EntityID EntityManager::CloneArchetype(EntityID id)
	{
		// Probably have to make additional changes to clone / make use of signatures
		return {};
	}

	auto EntityManager::GetEntitySignature(EntityID id) const -> Signature
	{
		//PPB_ASSERT_MSG(id >= MAX_ENTITIES,
		//			 "EntityManager::GetEntitySignature() -> Invalid ID: Out of range");

		return entity_signatures[id];
	}

	void EntityManager::SetEntitySignature(EntityID id, Signature sig)
	{
		//PPB_ASSERT_MSG(id >= MAX_ENTITIES,
		//			 "EntityManager::SetEntitySignature() -> Invalid ID: Out of range");

		entity_signatures[id] = sig;
	}

	auto EntityManager::GetArchetypeSignature(EntityID id) const -> Signature
	{
		//PPB_ASSERT_MSG(id >= MAX_ARCHETYPES,
		//			 "EntityManager::GetArchetypeSignature() -> Invalid ID: Out of range");

		return archetype_signatures[id];
	}

	void EntityManager::SetArchetypeSignature(EntityID id, Signature sig)
	{
		//PPB_ASSERT_MSG(id >= MAX_ARCHETYPES,
		//			 "EntityManager::SetArchetypeSignature() -> Invalid ID: Out of range");

		archetype_signatures[id] = sig;
	}

	// Entity* EntityManager::GetArchetype(const std::string& name) const
	//{
	//	
	//}

	void EntityManager::DestroyEntity(EntityID id)
	{
		//PPB_ASSERT_MSG(id >= MAX_ENTITIES,
		//			 "EntityManager::DestroyEntity() -> Invalid ID: Out of range");

		entity_signatures[id].reset();

		available_entities.push(id);
		--entity_count;
	}

	void EntityManager::DestroyArchetype(EntityID id)
	{
		//PPB_ASSERT_MSG(id >= MAX_ARCHETYPES,
		//			 "EntityManager::DestroyArchetype() -> Invalid ID: Out of range");

		archetype_signatures[id].reset();

		available_archetypes.push(id);
		--archetype_count;
	}

	void EntityManager::DestroyAllEntities()
	{
		for (EntityID id = 0; id < MAX_ENTITIES; ++id)
		{
			entity_signatures[id].reset();

			available_entities.push(id);
			--entity_count;
		}
	}

	void EntityManager::DestroyAllArchetypes()
	{
		for (EntityID id = 0; id < MAX_ARCHETYPES; ++id)
		{
			archetype_signatures[id].reset();

			available_archetypes.push(id);
			--archetype_count;
		}
	}

	void EntityManager::UpdateEntities()
	{
		while (!entity_delete_queue.empty())
		{
			EntityID id = entity_delete_queue.front();
			entity_delete_queue.pop();

			DestroyEntity(id);
		}

		while (!archetype_delete_queue.empty())
		{
			EntityID id = archetype_delete_queue.front();
			archetype_delete_queue.pop();

			DestroyArchetype(id);
		}
	}
}