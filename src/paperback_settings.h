#pragma once
namespace paperback::settings
{
	constexpr auto              max_component_types_v			    = 128;
	constexpr auto              max_components_per_entity_v		    = 64;
	constexpr auto              max_entities_per_pool_v			    = 5000;
	constexpr auto              max_entities_v					    = 5000;
	constexpr auto              virtual_page_size_v				    = 4096;
	constexpr auto              invalid_index_v					    = UINT32_MAX;
	constexpr static u32        invalid_delete_index_v              = 0xffffffffu >> 1;
}