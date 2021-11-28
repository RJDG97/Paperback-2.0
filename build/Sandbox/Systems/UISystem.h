#pragma once

struct ui_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "UI System"
    };

    using query = std::tuple< 
        paperback::query::must<transform, scale, rotation, mesh>
    ,   paperback::query::one_of<button, card>
    ,   paperback::query::none_of<prefab> 
    >;

    tools::query m_ButtonQuery;
    bool         m_Picked = false;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<Input::KeyPressed>( this );      // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<Input::KeyClicked>( this );      // Released                 ( Passes True )
        RegisterGlobalEventClass<Input::MousePressed>( this );    // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<Input::MouseClicked>( this );    // Released                 ( Passes True )

        m_ButtonQuery.AddQueryFromTuple( xcore::types::null_tuple_v< query > );
    }

    PPB_FORCEINLINE
    bool UICollided( transform& Transform, scale& Scale, const paperback::Vector3f MousePos ) const noexcept
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

    void operator()( transform& Transform, scale& Scale, mesh& Mesh, button* Button, card* Card ) noexcept
    {
        // Grab Mouse Coords
        auto pos = GetMousePositionInUI();

        if ( UICollided( Transform, Scale, paperback::Vector3f{ pos.x, pos.y, pos.z } ) )
        {
            if ( Button && Button->m_bActive )
            {
                Button->SetButtonState( ButtonState::HOVERED );
                // Mesh.m_Texture = Button->m_ButtonStateTextures[ Button->m_ButtonState ];

                // Run OnHover Script If Valid
                auto Script = FindScript<paperback::script::button_interface>( Button->m_ReferencedScript );
                if ( Script ) Script->OnHover();
            }
            else if ( Card )
            {
                // Update Card State
                Card->SetCardState( CardState::CLICKED );
                // Mesh.m_Texture = Card->m_CardStateTextures[ Card->m_ButtonState ];
                // 
                // Run OnClick Script If Valid
                auto Script = FindScript<paperback::script::card_interface>( Card->m_ReferencedScript );
                if ( Script ) Script->OnHover();
            }
        }
        else
        {
            if ( Button && Button->m_bActive )
            {
                Button->SetButtonState( ButtonState::DEFAULT );
                // Mesh.m_Texture = Button->m_ButtonStateTextures[ Button->m_ButtonState ];

                //// Run Default Script If Valid
                // auto Script = FindScript<paperback::script::button_interface>( Button->m_ReferencedScript );
                // if ( Script ) Script->Run();
            }
            else if ( Card )
            {
                Card->SetCardState( CardState::DEFAULT );
                // Mesh.m_Texture = Card->m_CardStateTextures[ Card->m_CardState ];

                //// Run Default Script If Valid
                // auto Script = FindScript<paperback::script::card_interface>( Card->m_ReferencedScript );
                // if ( Script ) Script->Run();
            }
        }
    }

    // On Event Key / Mouse Pressed
    void OnEvent( const size_t& Key, const bool& Clicked ) noexcept
    {
        if ( Key == GLFW_KEY_ESCAPE )
        {
            
        }
        else if ( Key == GLFW_MOUSE_BUTTON_1 )
        {
            auto MPos = GetMousePositionInUI();

            ForEach( Search( m_ButtonQuery ), [&]( entity& Entity, transform& Transform, scale& Scale, button* Button, card* Card, selected* Selected ) noexcept
            {
                if ( UICollided( Transform, Scale, paperback::Vector3f{ MPos.x, MPos.y, MPos.z } ) )
                {
                    // Key Pressed & Released
                    if ( Clicked )
                    {
                        // Button Is On The Active Layer
                        if ( Button && Button->m_bActive )
                        {
                            // Update Button State
                            Button->SetButtonState( ButtonState::CLICKED );
                            // Mesh.m_Texture = Button->m_ButtonStateTextures[ Button->m_ButtonState ];

                            // Run OnClick Script If Valid
                            auto Script = FindScript<paperback::script::button_interface>( Button->m_ReferencedScript );
                            if ( Script ) Script->OnClick();
                        }
                        // System Has Released A Card
                        else if ( m_Picked && Card && Selected && Selected->m_Value )
                        {
                            // On Placement of Card, Reset
                            m_Picked = false;
                            Selected->m_Value = false;

                            // Update Card State
                            Card->SetCardState( CardState::CLICKED );
                            // Mesh.m_Texture = Card->m_CardStateTextures[ Card->m_CardState ];

                            /*
                            Verify If Card Was Placed On Lane
                            Else:
                            Transform.m_Position = Card.m_OriginalPosition;
                            */

                            // Run OnClick Script If Valid
                            auto Script = FindScript<paperback::script::card_interface>( Card->m_ReferencedScript );
                            if ( Script ) Script->OnClick( Card->m_UnitGID, Card->m_PositionIndex );
                        }
                    }
                    // Key Pressed - Not Released Yet
                    else
                    {
                        // System Has Selected A Card
                        if ( Card && Selected && Selected->m_Value )
                        {
                            //// Offset between Cursor & Card Position
                            //auto Cursor = paperback::Vector3f{ MPos.x, MPos.y, 0.0f};
                            //auto Offset = Cursor - Transform.m_Position;
                            //Transform.m_Position = Cursor + Offset;

                            Transform.m_Position = paperback::Vector3f{ MPos.x, MPos.y, 0.0f };
                        }
                        // System Yet To Pick Card
                        else if ( !m_Picked && Card && Selected )
                        {
                            m_Picked = true;
                            Selected->m_Value = true;
                        }
                    }
                }
            });
        }
    }
};