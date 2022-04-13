/**********************************************************************************
*\file         paperback_cpp_script_mgr_inline.h
*\brief        paperback_cpp_script_mgr_inline.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

//-----------------------------------
//      Sample Button Script
//-----------------------------------
struct test_button_script : paperback::script::button_interface // Inherited Type (1)
{
    static constexpr auto typedef_v = paperback::script::type::button
                                      {
                                          .m_pName = "Test Button Script"
                                      };
    
    // You have to define this constructor - For internal registration
    test_button_script( paperback::coordinator::instance& Instance ) noexcept :
        // Override paperback::script::button_interface with the Inherited Type (1)
        paperback::script::button_interface{ Instance }
    { }

    // Override virtual functions defined within button_interface
    void OnHover( void ) noexcept override
    {
        //TRACE_PRINT( "Button Hovered!" );
    }

    void OnClick( void ) noexcept override
    {
        // auto list = m_Coordinator.Search<paperback::component::entity>();
        // TRACE_PRINT( "Button Clicked!" );
        // TRACE_PRINT( "Number Of Archetypes w/ Entity Component: " + std::to_string( list.size() ) );
    }
};

namespace paperback::script
{
    //-----------------------------------
    //             Default
    //-----------------------------------

    manager::manager( paperback::coordinator::instance& Coordinator ) noexcept :
        m_Coordinator{ Coordinator }
    {
        // Testing Script Register
        RegisterScripts<test_button_script>();

        //// Printing list of Guids & Script Names
        //auto& List = GetScriptsList();
        //for ( const auto& [info, script] : List )
        //{
        //    std::cout << "Script Name: " << info->m_pName
        //              << " | Script Guid: " << info->m_Guid.m_Value << std::endl;
        //}
    }


    //-----------------------------------
    //          Registration
    //-----------------------------------

    template < paperback::concepts::Script T_SCRIPT >
    void manager::RegisterScript( void ) noexcept
    {
        m_ScriptsList.push_back({ &script::info_v<T_SCRIPT> , std::make_unique<T_SCRIPT>( m_Coordinator ) });
        m_ScriptsMap.emplace( script::info_v<T_SCRIPT>.m_Guid.m_Value, m_ScriptsList.back().second.get() );
    }

    template < paperback::concepts::Script... T_SCRIPT >
    void manager::RegisterScripts( void ) noexcept
    {
        ( (RegisterScript<T_SCRIPT>()), ... );
    }


    //-----------------------------------
    //             Search
    //-----------------------------------

    template < paperback::concepts::Script T_SCRIPT >
    T_SCRIPT* manager::FindScript( const paperback::u64 ScriptGuid ) const noexcept
    {
        auto Script = m_ScriptsMap.find( ScriptGuid );
        return ( Script != m_ScriptsMap.end() ) ? static_cast<T_SCRIPT*>( Script->second )
                                                : nullptr;
    }

    template < paperback::concepts::Script T_SCRIPT >
    T_SCRIPT& manager::GetScript( const paperback::u64 ScriptGuid ) const noexcept
    {
        auto Script = FindScript<T_SCRIPT>( ScriptGuid );
        PPB_ASSERT_MSG( !Script, "Script is not registered yet" );
        return *Script;
    }


    //-----------------------------------
    //             Getter
    //-----------------------------------

    const manager::CPPScriptsList& manager::GetScriptsList( void ) const noexcept
    {
        return m_ScriptsList;
    }
}