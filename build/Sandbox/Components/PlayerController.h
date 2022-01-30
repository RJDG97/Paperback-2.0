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
	bool                              m_PlayerStatus = true;
	bool                              m_OnGround = true;;
};

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
		   .property( "Player Active Status", &player_controller::m_PlayerStatus )
		   .property( "Player On Ground Status", &player_controller::m_OnGround);
    }
}