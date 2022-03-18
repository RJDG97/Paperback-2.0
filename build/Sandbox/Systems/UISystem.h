#pragma once
#include "../Scripts/UI_Layers.h"

struct ui_system : paperback::system::pausable_instance
{

    bool m_FrameButtonLock{}; //used to ensure that only 1 onclick event can be processed per frame to prevent multi chain spam in a single frame

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "UI System"
    };

    using query = std::tuple< 
        paperback::query::must<transform, scale, rotation, mesh, name>
    ,   paperback::query::one_of<button, card, sound>
    ,   paperback::query::none_of<prefab> 
    >;

    tools::query m_ButtonQuery;
    tools::query m_AudioQuery;
    std::string  m_CurrentButtonHovered{};
    bool         m_Picked = false;

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        RegisterGlobalEventClass<paperback::input::manager::KeyPressed>( this );      // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<paperback::input::manager::KeyClicked>( this );      // Released                 ( Passes True )
        RegisterGlobalEventClass<paperback::input::manager::MousePressed>( this );    // Held Down - Not Released ( Passes False )
        RegisterGlobalEventClass<paperback::input::manager::MouseClicked>( this );    // Released                 ( Passes True )

        m_AudioQuery.m_Must.AddFromComponents < name >();
        m_AudioQuery.m_OneOf.AddFromComponents < sound, entity >();
        m_AudioQuery.m_NoneOf.AddFromComponents< prefab >();

        m_ButtonQuery.AddQueryFromTuple( xcore::types::null_tuple_v< query > );
    }

    PPB_FORCEINLINE
    bool UICollided( transform& Transform, scale& Scale, const paperback::Vector3f MousePos ) const noexcept
    {

        paperback::Vector3f screen_tf, screen_scale; //need to convert scale and transform to relative screen positions
        int width, height;

        auto WindowsSystem = PPB.FindSystem<window_system>();

        glfwGetWindowSize(WindowsSystem->m_pWindow, &width, &height);

        screen_tf.x = Transform.m_Position.x / (0.5f * width);
        screen_tf.y = Transform.m_Position.y / (0.5f * height);
        screen_scale.x = Scale.m_Value.x / (0.5f * width);
        screen_scale.y = Scale.m_Value.y / (0.5f * height);


        /*float btm_left_x { Transform.m_Position.x - ( Scale.m_Value.x * 0.5f ) }
            , btm_left_y { Transform.m_Position.y - ( Scale.m_Value.y * 0.5f ) }
            , top_right_x{ Transform.m_Position.x + ( Scale.m_Value.x * 0.5f ) }
            , top_right_y{ Transform.m_Position.y + ( Scale.m_Value.y * 0.5f ) };*/
        float btm_left_x { screen_tf.x - ( screen_scale.x * 0.5f ) }
            , btm_left_y { screen_tf.y - ( screen_scale.y * 0.5f ) }
            , top_right_x{ screen_tf.x + ( screen_scale.x * 0.5f ) }
            , top_right_y{ screen_tf.y + ( screen_scale.y * 0.5f ) };

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

        if ( UICollided( Transform, Scale, paperback::Vector3f{ pos.x, pos.y, 0.0f } ) )
        {
            if ( Button && Button->m_bActive )
            {
                auto UI_Sys = m_Coordinator.FindSystem<ui_system>();
                if ( UI_Sys && Button->IsButtonState( ButtonState::DEFAULT ) ) UI_Sys->TriggerSoundEntity("ButtonHoverSFX");

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
                auto Script = FindScript<paperback::script::button_interface>( Button->m_ReferencedScript );
                if ( Script ) Script->Run();
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
                if ( UICollided( Transform, Scale, paperback::Vector3f{ MPos.x, MPos.y, 0.0f } ) )
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
                            if ( Script ) Script->OnClick(Entity, Card->m_UnitGID, Card->m_PositionIndex );
                        }
                    }
                    // Key Pressed - Not Released Yet
                    else
                    {
                        // System Has Selected A Card
                        if ( Card && Selected && Selected->m_Value )
                        {
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

    PPB_INLINE
    void OnFrameEnd() noexcept
    {

        PPB.OpenQueuedScene();

        m_FrameButtonLock = false; //resets the lock
    }

    PPB_INLINE
    void OnStateChange() noexcept
    {

        m_FrameButtonLock = false;
        m_CurrentButtonHovered = "";
    }

    //given a layer, disable/enable all buttons with spe
    void ToggleLayerObjects(int LayerID, bool SetToggle) noexcept
    {

        ForEach(Search(m_ButtonQuery), [&](entity& Entity, button* Button, mesh* Mesh) noexcept
        {

            if (Button && Button->m_Group == LayerID && Mesh)
            {

                //do toggle code here
                Button->m_bActive = SetToggle;
                Mesh->m_Active = SetToggle;
            }
        });
    }

    //prevents more buttons clicks from being processed per frame
    bool SetButtonLock()
    {

        if (m_FrameButtonLock)
            return false;

        m_FrameButtonLock = true;

        return true;
    }

    bool SetHoverLock(const std::string& Compare)
    {

        if (Compare == m_CurrentButtonHovered)
            return false;

        m_CurrentButtonHovered = Compare;

        return true;
    }

    void TriggerSoundEntity(const std::string& EntityName)
    {

        ForEach(Search(m_AudioQuery), [&](entity& Entity, name& Name, sound* Sound) noexcept
        {

            if (Name.m_Value == EntityName && Sound)
            {

                Sound->m_Trigger = true;
                return;
            }
        });
    }

    void TriggerStopSoundEntity(const std::string& EntityName)
    {

        ForEach(Search(m_AudioQuery), [&](entity& Entity, name& Name, sound* Sound) noexcept
            {

                if (Name.m_Value == EntityName && Sound)
                {

                    //PPB.GetSystem<sound_system>().StopTriggeredSoundEvent(Sound->m_SoundPlayTag);
                    Sound->m_ForceStop = true;
                    return;
                }
            });
    }
};