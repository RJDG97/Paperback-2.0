/**********************************************************************************
*\file         Paperback.h
*\brief        Paperback.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef PAPERBACK_H
#define PAPERBACK_H

//----------------------------------
// Defines
//----------------------------------
#ifdef PAPERBACK_DEBUG
#define PPB_INLINE inline
#define PPB_FORCEINLINE __inline
#else
#define PPB_INLINE inline
#define PPB_FORCEINLINE __inline
#endif

// Define This To Enable/Disable Installer Build
//#define PAPERBACK_INSTALLER_BUILD

//----------------------------------
// STL
//----------------------------------
#define NOMINMAX
#include "Windows.h"
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <set>
#include <functional>
#include <typeindex>

//----------------------------------
// Dependencies
//----------------------------------
#include "../dependencies/xcore/src/xcore.h"

//----------------------------------
// Forward Declaration
//----------------------------------
namespace paperback
{
	namespace coordinator
	{
		class instance;
	}
	namespace system
	{
		struct system_interface;
		struct instance;
	}
	namespace script
	{
		struct script_interface;
	}
	namespace component
	{
		struct info;
	}
	namespace archetype
	{
		class manager;
	}

	namespace archetype
	{
		class instance;
	}

	namespace vm
	{
		struct PoolDetails;
	}
}

//----------------------------------
// Reflection
//----------------------------------
#include <rttr/registration>

//----------------------------------
// Files
//----------------------------------
#include "paperback_logger.h"
#include "paperback_types.h"
#include "paperback_event.h"
#include "paperback_concepts.h"
#include "paperback_event_mgr.h"
#include "paperback_settings.h"
#include "paperback_clock.h"
#include "paperback_bitset.h"
#include "paperback_nbitset.h"
#include "Input/paperback_input_binding.h"                  // New - Might Shift Directly Into Input
#include "Input/paperback_input_action.h"                   // New - Might Shift Directly Into Input
#include "Input/paperback_input_controller.h"               // New - Might Shift Directly Into Input
#include "paperback_input.h"
#include "paperback_event.h"
#include "paperback_component.h"
#include "paperback_component_mgr.h"

#include "Json/paperback_serialize.h"
#include "Json/paperback_deserialize.h"
#include "Json/paperback_json.h"

#include "paperback_query.h"
#include "Physics/paperback_physics_node.h"
#include "Physics/paperback_physics_aabbtree.h"
#include "../../build/Sandbox/Components/Parent.h"
#include "../../build/Sandbox/Components/Child.h"
#include "paperback_pool.h"
#include "paperback_archetype.h"
#include "paperback_archetype_mgr.h"
#include "paperback_cppscript_mgr.h"
#include "paperback_particle_mgr.h"
#include "paperback_system.h"
#include "paperback_system_mgr.h"
#include "paperback_statemgr.h"
#include "paperback_coordinator.h"


//----------------------------------
// Inline Files
//----------------------------------
#include "Details/paperback_logger_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_event_mgr_inline.h"
#include "Details/paperback_clock_inline.h"
#include "Input/Details/paperback_input_action_inline.h"	// New - Might Shift Directly Into Input
#include "Input/States/paperback_input_state_includes.h"	// New - Might Shift Directly Into Input
#include "Input/Devices/paperback_device_includes.h"		// New - Might Shift Directly Into Input
#include "Details/paperback_input_inline.h"
#include "Details/paperback_component_inline.h"
#include "Details/paperback_component_mgr_inline.h"
#include "Details/paperback_system_inline.h"
#include "Details/paperback_system_mgr_inline.h"

#include "Json/paperback_serialize_inline.h"
#include "Json/paperback_deserialize_inline.h"
#include "Json/paperback_json_inline.h"

#include "Physics/paperback_physics_aabbtree_inline.h"
#include "Details/paperback_pool_inline.h"
#include "Details/paperback_archetype_inline.h"
#include "Details/paperback_archetype_mgr_inline.h"
#include "Details/paperback_cppscript_mgr_inline.h"
#include "Details/paperback_particle_mgr_inline.h"
#include "Details/paperback_query_inline.h"
#include "Details/paperback_bitset_inline.h"
#include "Details/paperback_nbitset_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_coordinator_inline.h"
#include "Details/paperback_statemgr_inline.h"

#include "paperback_data_reflections.h"

#endif