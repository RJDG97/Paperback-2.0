#pragma once

struct ui_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "UI System"
    };

    using query = std::tuple< 
        paperback::query::must<transform, scale, rotation, mesh, button>
    ,   paperback::query::one_of<>
    ,   paperback::query::none_of<prefab> 
    >;

    tools::query m_ButtonQuery;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );
        RegisterGlobalEventClass<Input::MousePressed>( this );

        m_ButtonQuery.AddQueryFromTuple( xcore::types::null_tuple_v< query > );
    }

    PPB_FORCEINLINE
    bool ButtonCollided( transform& Transform, scale& Scale, const paperback::Vector3f MousePos ) const noexcept
    {
        float btm_left_x { Transform.m_Position.x - ( Scale.m_Value.x * 0.5f ) }
            , btm_left_y { Transform.m_Position.y - ( Scale.m_Value.y * 0.5f ) }
            , top_right_x{ Transform.m_Position.x + ( Scale.m_Value.x * 0.5f ) }
            , top_right_y{ Transform.m_Position.y + ( Scale.m_Value.y * 0.5f ) };

        // Verify if within button
	    if ( btm_left_x  <= MousePos.x && btm_left_y  <= MousePos.y &&
		     top_right_x >= MousePos.x && top_right_y >= MousePos.y )
		    return true;
	    return false;
    }

    void operator()( transform& Transform, scale& Scale, mesh& Mesh, button& Button ) noexcept
    {
        auto pos = GetMousePositionInUI();

        if ( ButtonCollided( Transform, Scale, paperback::Vector3f{ pos.x, pos.y, pos.z } ) )
        {
            Button.SetButtonState( ButtonState::HOVERED );
            // Mesh.m_Texture = Button.m_ButtonStateTextures[ Button.m_ButtonState ];

            // Run OnHover Script If Valid
            auto Script = FindScript<paperback::script::button_interface>( Button.m_ReferencedScript );
            if ( Script ) Script->OnHover();
        }
        else
        {
            Button.SetButtonState( ButtonState::DEFAULT );
            // Mesh.m_Texture = Button.m_ButtonStateTextures[ Button.m_ButtonState ];

            //// Run Default Script If Valid
            // auto Script = FindScript<paperback::script::button_interface>( Button.m_ReferencedScript );
            // if ( Script ) Script->Run();
        }
    }

    // On Event Key / Mouse Pressed
    void OnEvent( const size_t& Key ) noexcept
    {
        if ( Key == GLFW_KEY_ESCAPE )
        {
            
        }
        else if ( Key == GLFW_MOUSE_BUTTON_1 )
        {
            auto pos = GetMousePositionInUI();

            ForEach( Search( m_ButtonQuery ), [&]( transform& Transform, scale& Scale, button& Button ) noexcept
            {
                if ( ButtonCollided( Transform, Scale, paperback::Vector3f{ pos.x, pos.y, pos.z } ) && Button.m_bActive )
                {
                    // Update Button State
                    Button.SetButtonState( ButtonState::CLICKED );
                    // Run OnClick Script If Valid
                    auto Script = FindScript<paperback::script::button_interface>( Button.m_ReferencedScript );
                    if ( Script ) Script->OnClick();
                }
            });
        }
    }
};