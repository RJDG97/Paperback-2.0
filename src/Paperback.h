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

//----------------------------------
// STL
//----------------------------------
#define NOMINMAX
#include "Windows.h"
#include <iostream>
#include <vector>
#include <array>
#include <queue>
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
	namespace component
	{
		struct info;
	}
	namespace archetype
	{
		class manager;
	}

}

//----------------------------------
// Reflection
//----------------------------------
#include <rttr/registration>
#include "paperback_data_reflections.h"

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
#include "paperback_input.h"
#include "paperback_event.h"
#include "paperback_component.h"
#include "paperback_component_mgr.h"

#include "Json/paperback_serialize.h"
#include "Json/paperback_deserialize.h"
#include "Json/paperback_json.h"

#include "paperback_bitset.h"
#include "paperback_query.h"
#include "../../build/Sandbox/Components/Parent.h"
#include "../../build/Sandbox/Components/Child.h"
#include "paperback_pool.h"
#include "paperback_archetype.h"
#include "paperback_archetype_mgr.h"
#include "paperback_system.h"
#include "paperback_system_mgr.h"
#include "paperback_coordinator.h"


//----------------------------------
// Inline Files
//----------------------------------
#include "Details/paperback_logger_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_event_mgr_inline.h"
#include "Details/paperback_clock_inline.h"
#include "Details/paperback_input_inline.h"
#include "Details/paperback_component_inline.h"
#include "Details/paperback_component_mgr_inline.h"
#include "Details/paperback_system_inline.h"
#include "Details/paperback_system_mgr_inline.h"

#include "Json/paperback_serialize_inline.h"
#include "Json/paperback_deserialize_inline.h"
#include "Json/paperback_json_inline.h"

#include "Details/paperback_pool_inline.h"
#include "Details/paperback_archetype_inline.h"
#include "Details/paperback_archetype_mgr_inline.h"
#include "Details/paperback_query_inline.h"
#include "Details/paperback_bitset_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_coordinator_inline.h"

//----------------------------------
// Math files
//----------------------------------
//#include "Math/Math_includes.h"







#endif