#pragma once

struct player_controller
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Player Controller"
	};

	std::vector<paperback::u64>       m_ControllerGuid{ };
	float                             m_MovementForce = 3.0f;
	float                             m_JumpForce = 0.0f;
	float                             m_CameraRotationSpeed = 0.0f;
	float                             m_CameraRadius = 0.0f;
	bool                              m_PlayerStatus = true;
	bool                              m_OnGround = true;;
	bool                              m_FPSMode = false;
	bool                              m_GrowAvailable = false;
	bool                              m_ShrinkAvailable = false;
	bool                              m_FreezeAvailable = false;
};


// Data Component Creation Sample
/*
BEGIN_CREATE_DATA_COMPONENT( player_controller )

	std::vector<paperback::u64>       m_ControllerGuid{ };
	float                             m_MovementForce = 3.0f;
	float                             m_JumpForce = 0.0f;
	float                             m_CameraRotationSpeed = 0.0f;
	float                             m_CameraRadius = 0.0f;
	bool                              m_PlayerStatus = true;
	bool                              m_OnGround = true;;
	bool                              m_FPSMode = false;
	
END_CREATE_DATA_COMPONENT
*/


namespace RR_PlayerController
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<player_controller>( player_controller::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Device Guid", &player_controller::m_ControllerGuid )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Movement Force", &player_controller::m_MovementForce )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Jump Force", &player_controller::m_JumpForce )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Camera Rotation Speed", &player_controller::m_CameraRotationSpeed )( rttr::policy::prop::as_reference_wrapper )
		   .property( "Camera Radius", &player_controller::m_CameraRadius )
		   .property( "Player Active Status", &player_controller::m_PlayerStatus )
		   .property( "Player On Ground Status", &player_controller::m_OnGround)
		   .property( "Player FPS Status", &player_controller::m_FPSMode)
		   .property( "Player Grow Available", &player_controller::m_GrowAvailable)(rttr::policy::prop::as_reference_wrapper)
		   .property( "Player Shrink Available", &player_controller::m_ShrinkAvailable)(rttr::policy::prop::as_reference_wrapper)
		   .property( "Player Freeze Available", &player_controller::m_FreezeAvailable)(rttr::policy::prop::as_reference_wrapper);
    }
}