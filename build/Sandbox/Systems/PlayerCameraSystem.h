#pragma once
#include "../src/paperback_camera.h"

struct player_camera_system : paperback::system::pausable_instance
{
    tools::query Query;

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "player_camera_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        Query.m_Must.AddFromComponents<entity, transform, player_controller, camera>();
        Query.m_NoneOf.AddFromComponents<prefab>();
    }

    PPB_FORCEINLINE
    void Update( void ) noexcept
    {
        // Populate map
        ForEach( Search( Query ), [&]( entity& PlayerEntity, transform& Transform, player_controller& Controller, camera& Camera ) noexcept
        {
            if ( Controller.m_PlayerStatus && Camera.m_Active )
            {
                // Exclude Player From Query
                std::vector<paperback::u32> ExcludeList;
                ExcludeList.push_back( PlayerEntity.m_GlobalIndex );

                auto Direction = paperback::Vector3f{ Camera.m_Position.x, Camera.m_Position.y, Camera.m_Position.z } - Transform.m_Position;
                auto NormDir   = Direction.Normalized( );
                auto MaxCamPos = Transform.m_Position + NormDir * Camera.m_MaxRadius;

                auto [Hit_ID, HitDist] = m_Coordinator.QueryRaycastClosest( Transform.m_Position         // Start Ray
                                                                          , MaxCamPos                    // End Ray
                                                                          , ExcludeList );               // Excluded Entities

                // There Exists Closest Entity In Camera Range - Limit Camera Radius
                if ( Hit_ID != paperback::settings::invalid_index_v )
                {
                    auto Rate      = Camera.m_ZoomRate * DeltaTime();
                    auto NewRadius = Camera.m_MaxRadius * HitDist;
                    NewRadius = NewRadius > Camera.m_MaxRadius ? Camera.m_MaxRadius : NewRadius;
                    NewRadius = NewRadius < Camera.m_MinRadius ? Camera.m_MinRadius : NewRadius;

                    // Zoom Out
                    if ( NewRadius > Camera.m_Radius &&
                        (Camera.m_Radius + Rate) < Camera.m_MaxRadius &&
                        (NewRadius - Camera.m_Radius) > 0.05f )
                        Camera.m_Radius += Rate;

                    // Zoom In
                    else if ( NewRadius < Camera.m_Radius &&
                             (Camera.m_Radius - Rate) > Camera.m_MinRadius &&
                             (Camera.m_Radius-NewRadius) > 0.05f )
                        Camera.m_Radius -= Rate;
                }
            }
        });
    }
};