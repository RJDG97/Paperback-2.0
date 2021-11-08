#pragma once

namespace paperback
{
    //----------------------------------
	// Concepts C++20
	//----------------------------------
	namespace concepts
	{
		template < typename T_SYSTEM >
		concept System						= std::derived_from< T_SYSTEM, system::instance >;

		template < typename T_EVENT >
		concept Event						= std::is_base_of_v< paperback::event::event_interface, T_EVENT >;

		template < typename T_SYSTEM >
		concept HasEvent					= !( std::is_same_v< typename T_SYSTEM::events, system::system_interface::events > );

		template< typename T_FUNCTION = empty_lambda >
		concept Callable					= xcore::function::is_callable_v< T_FUNCTION >;

		template< typename T_FUNCTION >
		concept Callable_Void				= Callable< T_FUNCTION >
										   && std::is_same_v< void, FN_RETURN_TYPE< T_FUNCTION > >;

		template< typename T_FUNCTION >
		concept Callable_Bool				= Callable< T_FUNCTION >
										   && std::is_same_v< bool, FN_RETURN_TYPE< T_FUNCTION > >;

		template < typename T_CONTAINER >
		concept TupleSpecialization			= xcore::types::is_specialized_v< std::tuple, T_CONTAINER >;

		template < typename... T_ARGS >
		concept ReferenceArgs				= ( std::is_reference_v<T_ARGS> && ... );

		template < typename... T_ARGS >
		concept MixedArgs					= !( std::is_reference_v<T_ARGS> && ... );
	}
}