#pragma once

#ifndef PPB_INPUT_BINDING
#define PPB_INPUT_BINDING


namespace paperback::input::binding
{
    //-----------------------------------
    //           Input Types
    //-----------------------------------

    namespace type
    {
        using guid = xcore::guid::unit<64, struct input_tag>;

        struct control_binding
        {
            binding::type::guid      m_Guid{ };
            const char*              m_pName{ };
        };

        //-----------------------------------
        //              Info
        //-----------------------------------

        struct info
		{
			const binding::type::guid    m_Guid;
			const char*					 m_pName{ };
		};
    }


    namespace details
    {
        template< typename T_CLASS >
        consteval binding::type::info CreateInfo( void ) noexcept
        {
            return binding::type::info
            {
                .m_Guid                 = T_CLASS::typedef_v.m_Guid.m_Value
                                          ? T_CLASS::typedef_v.m_Guid
                                          : type::guid{ __FUNCSIG__ }
            ,   .m_pName                = T_CLASS::typedef_v.m_pName
            };
        }
    
        template< typename T_CLASS >
        static constexpr auto info_v = binding::details::CreateInfo<T_CLASS>();
    }
    
    template< typename T_CLASS >
    constexpr auto& info_v = input::binding::details::info_v< T_CLASS >;


    //-----------------------------------
    //          Input Binding
    //-----------------------------------

    struct instance { };

    /****************************************************************************************
                                    ** BINDING CONSTRUCT **

      binding_construct   |   Construct's Name
                          |
      To Do               |   Define OnInputAction( paperback::u32 GlobalIndex
                                                  , float Modifier ) noexcept

    ****************************************************************************************/

    #define BEGIN_BINDING_CONSTRUCT( binding_construct )                                               \
        struct binding_construct : binding::instance                                                   \
        {                                                                                              \
            static constexpr auto typedef_v = binding::type::control_binding                           \
                                              {                                                        \
                                                  .m_pName = #binding_construct                        \
                                              };                                                       \
                                                                                                       \
            PPB_INLINE                                                                                 \
            binding_construct( paperback::coordinator::instance& Coordinator ) :                       \
                m_Coordinator{ Coordinator }                                                           \
            { }                                                                                        \
                                                                                                       \
            paperback::coordinator::instance&     m_Coordinator;                                       \

    #define END_BINDING_CONSTRUCT                                                                      \
        };
    

    //-----------------------------------
    //      Input Binding Callback
    //-----------------------------------

    struct InputBinding
    {
        using Callback = void( void*, const float& );

        Callback*    m_pAction;
        void*        m_pClass;
    };
    
}

#endif