#pragma once

/*
To Use:

	1. Create a Component - In the future there will be data / tag / shared component types:
		
		struct my_component
        {
            constexpr static auto typedef_v = paperback::component::type::data
            {
                .m_pName = "my_component"
            };

	        // Data members
        };


	2. Register Component
		
		PPB.RegisterComponents<
			my_component
		>();
*/

namespace paperback::component
{
    //-----------------------------------
    //         Component Types
    //-----------------------------------
    namespace type
    {
        using guid = xcore::guid::unit<64, struct component_tag>;

        enum class id : u8
        {
            DATA = 0
        ,   TAG
        };


        //-----------------------------------
	    //    Component Containing Data
	    //-----------------------------------
        struct data
        {
            static constexpr auto max_size_v = settings::virtual_page_size_v;
            static constexpr auto id_v       = id::DATA;

            type::guid                         m_Guid { };
            const char*                        m_pName{ };
        };


        //-----------------------------------
        //      Component Of Size "0"
        //-----------------------------------
        struct tag
        {
            static constexpr auto max_size_v = 1;
            static constexpr auto id_v       = id::TAG;

            type::guid                         m_Guid { };
            const char*                        m_pName{ };
        };


        //-----------------------------------
        //  Component Registration Details
        //-----------------------------------
        namespace details
        {
            template< typename T_COMPONENT >
            struct is_valid
            {
                template< auto Class >         struct Check;
                template< typename >           static std::false_type VerifyComponent( ... );
                template< typename COMPONENT > static auto VerifyComponent( Check<&COMPONENT::typedef_v>* ) -> std::conditional_t
                                               <
                                                  ( std::is_same_v< const data, decltype( COMPONENT::typedef_v ) > && sizeof( T_COMPONENT ) <= data::max_size_v )
                                               || ( std::is_same_v< const tag,  decltype( COMPONENT::typedef_v ) > && sizeof( T_COMPONENT ) <= tag::max_size_v  )
                                               ,    std::true_type
                                               ,    std::false_type
                                               >;
                constexpr static auto value = decltype( VerifyComponent<T_COMPONENT>(nullptr) )::value;
            };

            template< typename T_COMPONENT >
            struct is_valid_data_component
            {
                template< auto Class >         struct Check;
                template< typename >           static std::false_type VerifyComponent( ... );
                template< typename COMPONENT > static auto VerifyComponent( Check<&COMPONENT::typedef_v>* ) -> std::conditional_t
                                               <
                                                  ( std::is_same_v< const data, decltype( COMPONENT::typedef_v ) > )
                                               ,    std::true_type
                                               ,    std::false_type
                                               >;
                constexpr static auto value = decltype( VerifyComponent<T_COMPONENT>(nullptr) )::value;
            };
        }

        template< typename T_COMPONENT >
        constexpr bool is_valid_v = details::is_valid< paperback::BaseType<T_COMPONENT> >::value;

        template< typename T_COMPONENT >
        constexpr bool is_valid_data_v = details::is_valid_data_component< paperback::BaseType<T_COMPONENT> >::value;
    }


    //-----------------------------------
    //      Component Information
    //-----------------------------------
    struct info
    {
        static constexpr auto invalid_id_v = 0xffff;

        using Constructor   =  void( std::byte* ) noexcept;
        using Destructor    =  void( std::byte* ) noexcept;
        using Move          =  void( std::byte* Destination, std::byte* Source ) noexcept;

        const type::guid       m_Guid;
        const type::id         m_TypeID;
        mutable u32            m_UID;
        mutable u32            m_Size;
        Constructor*           m_Constructor;
        Destructor*            m_Destructor;
        Move*                  m_Move;
        const char*            m_pName;
    };

    
    //-----------------------------------
    //       Get Component Info
    //-----------------------------------
    namespace details
    {
        template< typename T_COMPONENT >
        consteval info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = component::details::CreateInfo<T>();
    }

    template< typename T_COMPONENT >
    constexpr auto& info_v = details::info_v< paperback::BaseType<T_COMPONENT> >;


    //-----------------------------------
    //  Get array of Component Infos
    //-----------------------------------
    template < typename T_TUPLE >
	static constexpr auto info_array_v = []<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS>* )
	{
		if constexpr ( sizeof...(T_COMPONENTS) == 0 )   return std::span<const component::info*>{};
		else                                            return std::array{ &component::info_v<T_COMPONENTS>... };
	}( xcore::types::null_tuple_v< T_TUPLE > );
	

    //-----------------------------------
    //        Component: Entity
    //-----------------------------------
    union entity final
    {
        constexpr static auto typedef_v = paperback::component::type::data
        {
            .m_pName = "Entity"
        };

        union Validation final
        {
            uint32_t        m_UID;
            struct
            {
                uint32_t    m_Next       : 31   // Index of next Entity within Entity Manager (For Deletion) - Currently Unused
                ,           m_bZombie    : 1;   // Entity Status
            };

            PPB_FORCEINLINE
            constexpr bool operator == ( const Validation& V ) const noexcept;
        };
        static_assert( sizeof(Validation) == sizeof(uint32_t) );

        uint64_t            m_UID;
        struct
        {
            uint32_t        m_GlobalIndex;      // Index of Entity within Entity Manager
            Validation      m_Validation;       // Entity Status
        };

        PPB_FORCEINLINE
        constexpr bool IsZombie( void ) const noexcept;

        PPB_FORCEINLINE
        constexpr bool operator == ( const entity& Entity ) const noexcept;
    };
    static_assert( sizeof(entity) == sizeof(uint64_t) );

    namespace RR_Entity
    {
        RTTR_REGISTRATION
        {
            rttr::registration::class_< entity >( entity::typedef_v.m_pName )
                .property( "Global Index", &entity::m_GlobalIndex )
                .property( "Validation", &entity::m_Validation );

            rttr::registration::class_< entity::Validation >( "Valid" )
                .property( "Validation ID", &entity::Validation::m_UID );
        }
    }
}