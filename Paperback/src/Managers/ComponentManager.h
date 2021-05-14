#pragma once

#include "PaperbackPCH.h"
#include "Paperback/Core.h"
#include "Components/Transform.h"

namespace Paperback
{
	// Redefine value to expand Component Array size
	static const u32 MAX_COMPONENTS = 1500;


	template <typename T>
	class CMap
	{
	public:

		using ComponentArray = std::array<Ref<T>, MAX_COMPONENTS>;
		using ComponentArrayIt = typename ComponentArray::iterator;

		using EntityToComponentUMap = std::unordered_map<EntityID, ComponentIndex>;
		using EntityToComponentUMapIt = EntityToComponentUMap::iterator;

		using ComponentUMapToEntity = std::unordered_map<ComponentIndex, EntityID>;
		using ComponentUMapToEntityIt = ComponentUMapToEntity::iterator;

		void AddComponent(EntityID id, Ref<T> component)
		{
			PPB_ASSERT_MSG(entity_to_index_map.find(id) != entity_to_index_map.end(),
				"CMap<T>::AddComponent() -> Component already registered");

			entity_to_index_map[id] = component_count;
			index_to_entity_map[component_count] = id;
			component_array[component_count] = component;
			++component_count;
		}

		void RemoveComponent(EntityID id)
		{
			PPB_ASSERT_MSG(entity_to_index_map.find(id) == entity_to_index_map.end(),
				"CMap<T>::RemoveComponent() -> Component does not exist");

			// Swap "Component Data" of deleted entity with last valid entity
			ComponentIndex index_of_deleted = entity_to_index_map[id];
			u32 last_index = component_count - 1;
			component_array[index_of_deleted] = component_array[last_index];

			// Swap "ID and Index" of deleted entity with last valid entity
			EntityID id_of_last = index_to_entity_map[last_index];
			entity_to_index_map[id_of_last] = index_of_deleted;
			index_to_entity_map[index_of_deleted] = id_of_last;

			entity_to_index_map.erase(id);
			index_to_entity_map.erase(last_index);
			--component_count;
		}

		T* GetComponent(EntityID id)
		{
			PPB_ASSERT_MSG(entity_to_index_map.find(id) == entity_to_index_map.end(),
				"CMap<T>::GetComponent() -> Component does not exist");

			return std::reinterpret_pointer_cast<T>(component_array[entity_to_index_map[id]]).get();
		}

		void Init()
		{

		}

		size_t size() const
		{
			return component_array.size();
		}

		void clear()
		{
			for (size_t i = 0; i < component_count; ++i)
			{
				component_array[i] = {};
			}
			component_count = 0;
		}

		ComponentArrayIt begin() const
		{
			return component_array.begin();
		}

		ComponentArrayIt end() const
		{
			component_array.end();
		}

		CMap() :
			component_count{ }, component_array{ },
			entity_to_index_map{ }, index_to_entity_map{ }
		{ }

		~CMap()
		{

		}

	private:

		u32 component_count;
		ComponentArray component_array;
		EntityToComponentUMap entity_to_index_map;
		ComponentUMapToEntity index_to_entity_map;
	};


	template <typename... Ts>
	class CManager : private CMap<Ts>...
	{
	public:

		friend class Application;

		using ComponentIndex = std::unordered_map<std::string, ComponentType>;

		CManager() :
			next_component_id{ }, component_indexes{ }
		{
			RegisterComponent<Transform>();
		}

		void Init()
		{

		}

		~CManager()
		{

		}

		template <typename T>
		void RegisterComponent()
		{
			auto name = typeid(T).name();

			PPB_ASSERT_MSG(component_indexes.find(name) != component_indexes.end(),
				"CManager<Ts...>::RegisterComponent() -> Component already registered");

			component_indexes.insert({ name, next_component_id });
			// Replace with ComponentArray creation

			++next_component_id;
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			auto name = typeid(T).name();

			PPB_ASSERT_MSG(component_indexes.find(name) == component_indexes.end(),
				"CManager<Ts...>::GetComponentType() -> Component not registered");

			return component_indexes[name];
		}

		template <typename T>
		CMap<T>* GetComponentList() // Might not be used and incorrect
		{
			return this;
		}

		template <typename T>
		void AddComponent(EntityID id, Ref<T> component)
		{
			CMap<T>::AddComponent(id, component);
		}

		template <typename T>
		void RemoveComponent(EntityID id)
		{
			CMap<T>::RemoveComponent(id);
		}

		template <typename T>
		T* GetComponent(EntityID id)
		{
			return CMap<T>::GetComponent(id);
		}

		template <typename T>
		typename CMap<Ref<T>>* GetComponentArray() // Might not be used and incorrect
		{
			return this;
		}

	private:

		ComponentType next_component_id;
		ComponentIndex component_indexes;
	};


	// Add on to the definition for each Component Created
	using ComponentManager = CManager<
		Transform
	>;
}