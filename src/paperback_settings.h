#pragma once

namespace paperback::settings
{
	constexpr auto              max_component_types_v			    = 128;
	constexpr auto              max_components_per_entity_v		    = 64;
	constexpr auto              max_entities_per_pool_v			    = 5000;
	constexpr auto              max_entities_v					    = 5000;
	constexpr auto              max_partition_cells_v               = 128;
	constexpr auto              partition_cell_size_v               = 7;
	constexpr auto              batched_update_delay_v              = 5;
	constexpr auto              virtual_page_size_v				    = 4096;
	constexpr auto              invalid_index_v					    = UINT32_MAX;
	constexpr auto              invalid_controller_v                = GLFW_JOYSTICK_LAST + 1;
	constexpr auto              velocity_axis_cap_v                 = 15.0f;
	constexpr auto              momentum_axis_cap_v                 = 25.0f;
	constexpr static u32        invalid_delete_index_v              = 0xffffffffu >> 1;
}