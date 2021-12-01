#pragma once

enum class ButtonState : int
{
    DEFAULT = 0
,   HOVERED
,   CLICKED
};

enum class LayerState : int
{
    DECREASE = 0
,   NEUTRAL
,   INCREASE
};

struct button
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Button"
    };

    void SetButtonState( const ButtonState State ) noexcept
	{
		m_ButtonState = static_cast<int>( State );
	}

	bool IsButtonState( const ButtonState State ) const noexcept
	{
		return State == static_cast< ButtonState >( m_ButtonState );
	}

    LayerState GetLayerState( void ) const noexcept
	{
		return static_cast< LayerState >( m_LayerState );
	}

    bool IsLayerState( const LayerState State ) const noexcept
	{
		return State == static_cast< LayerState >( m_LayerState );
	}


    std::array<std::string, 3> m_ButtonStateTextures  { };                                             // Stores different state textures - Default, Hovered, Clicked
    int                        m_ButtonState          = static_cast<int>( ButtonState::DEFAULT );      // Current button state
    int                        m_LayerState           = static_cast<int>( LayerState::NEUTRAL );       // Current button state
    int                        m_Group                = 0;                                             // Layer group for buttons         - Displayed depending on current UI Layer
    paperback::u64             m_ReferencedScript     = 0;                                             // Guid of referenced script       - Script is stored within Script Manager
    bool                       m_bActive              = true;                                          // Render / Don't render (Check rendering side)
};

namespace RR_Button
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<button>( button::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Button Textures", &button::m_ButtonStateTextures )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Button State", &button::m_ButtonState )
		   .property( "Layer Grouping", &button::m_Group )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Script Guid", &button::m_ReferencedScript )(rttr::policy::prop::as_reference_wrapper)
		   .property( "Active Status", &button::m_bActive)( rttr::policy::prop::as_reference_wrapper );
    }
}