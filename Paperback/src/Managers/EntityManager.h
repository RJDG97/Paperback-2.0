#pragma once

#include "PaperbackPCH.h"
#include "Paperback/IComponent.h"

namespace Paperback
{
	// Redefine value to expand Entity & Archetype Array size
	const u32 MAX_ENTITIES = 2000;
	const u32 MAX_ARCHETYPES = 30;

	class PAPERBACK_API EntityManager
	{
	public:

		friend class Application;

		using EntityList = std::queue<EntityID>;
		using ArchetypeList = std::queue<EntityID>;
		using EntitySignature = std::array<Signature, MAX_ENTITIES>;		// Array of Size = MAX_ENTITIES (2000)
		using ArchetypeSignature = std::array<Signature, MAX_ARCHETYPES>;	// Array of Size = MAX_ARCHETYPES (30)

		EntityManager();
		~EntityManager();

		void Init();
		// void Deserialize();

		// Create / Clone
		EntityID CreateEntity();
		EntityID CreateArchetype();
		EntityID CloneEntity(EntityID id);
		EntityID CloneArchetype(EntityID id);

		// Signatures
		Signature GetEntitySignature(EntityID id) const;
		void SetEntitySignature(EntityID id, Signature sig);
		Signature GetArchetypeSignature(EntityID id) const;
		void SetArchetypeSignature(EntityID id, Signature sig);

		// Destroy
		void DestroyEntity(EntityID id);
		void DestroyArchetype(EntityID id);
		void DestroyAllEntities();
		void DestroyAllArchetypes();

		// Extras
		void UpdateEntities();
		// Entity* GetArchetype(const std::string& name) const;

	private:

	#pragma warning(disable : 4251)
		// Delete Queue
		EntityList entity_delete_queue;
		EntityList archetype_delete_queue;

		// Entities
		u16 entity_count;
		ArchetypeList available_entities;
		EntitySignature entity_signatures;

		// Archetypes
		u8 archetype_count;
		ArchetypeList available_archetypes;
		ArchetypeSignature archetype_signatures;
	#pragma warning(default : 4251)
	};
}