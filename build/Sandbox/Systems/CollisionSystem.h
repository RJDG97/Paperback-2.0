#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"
#include "Physics/ResolveCollision.h"

struct collision_system : paperback::system::pausable_instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "collision_system"
    };

    struct OnCollisionEnter       : paperback::event::instance< entity& , entity&, rigidforce&, rigidforce&, bool& > {};
    struct OnCollisionStay        : paperback::event::instance< entity&, entity&, rigidforce&, rigidforce&, boundingbox&, boundingbox&, bool& > {};
    struct OnCollisionExit        : paperback::event::instance< entity&, rigidforce&, bool& > {};

    using query = std::tuple
    <
        paperback::query::none_of<prefab>
    >;


    tools::query SphereColliderQuery;
    scripting_system* scripting_sys;


    PPB_FORCEINLINE
	void OnSystemCreated( void ) noexcept
	{
        scripting_sys = GetSystem<scripting_system>();
		SphereColliderQuery.m_Must.AddFromComponents < transform, sphere, rigidforce >();
        SphereColliderQuery.m_OneOf.AddFromComponents< mass >();
        SphereColliderQuery.m_NoneOf.AddFromComponents< prefab >();
	}


    void operator()( paperback::component::entity& Entity, transform& Transform, rigidforce& RigidForce, boundingbox* Boundingbox, sphere* Sphere, mass* m1 ) noexcept
    {
        if ( Entity.IsZombie() ) return;
       
        // Initialize Query
        bool SkipUnit = false;
        bool NotCollided = true;
             
        if ( Boundingbox )
        {
            Boundingbox->m_Collided = false;

            auto NeighbourList = m_Coordinator.SearchNeighbours( Transform.m_Position, Boundingbox->Min, Boundingbox->Max );

            ForEach( NeighbourList, [&]( entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, mass* m2 )
            {
                if ( Entity.IsZombie() || Dynamic_Entity.IsZombie() ) return;

                // Do not check against self
                if ( &Entity != &Dynamic_Entity )
                {
                    // Add to collision map
                    auto map = Boundingbox->m_CollisionState.find(Dynamic_Entity.m_GlobalIndex);
                    if (map == Boundingbox->m_CollisionState.end()) {
                        const auto& [map, Valid] = Boundingbox->m_CollisionState.insert({ Dynamic_Entity.m_GlobalIndex, false });
                    }
                
                    // Collision Detection
                    if ( Boundingbox && BB )
                    {
                        // If Both Entities Are Colliding Already
                        if ( AabbAabb( Transform.m_Position + Boundingbox->Min, Transform.m_Position + Boundingbox->Max
                                     , Xform.m_Position + BB->Min, Xform.m_Position + BB->Max ) )
                        {
                            if ( RF )
                            {
                                // Current Entity is NOT Colliding with Other Entity
                                if (!Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex)) {
                                    
                                    for (auto& to_update : scripting_sys->scriptlist[Entity.m_GlobalIndex].m_Info)
                                    {
                                        to_update.second->OnCollisionEnter(Dynamic_Entity.m_GlobalIndex);
                                    }

                                    //BroadcastGlobalEvent<OnCollisionEnter>(Entity, Dynamic_Entity, RigidForce, *RF, SkipUnit);
                                }
                                // Current Entity is ALREADY Colliding with Other Entity
                                else
                                {
                                    for (auto& to_update : scripting_sys->scriptlist[Entity.m_GlobalIndex].m_Info)
                                    {
                                        to_update.second->OnCollisionStay(Dynamic_Entity.m_GlobalIndex);
                                    }
                                }
                                    //BroadcastGlobalEvent<OnCollisionStay>( Entity, Dynamic_Entity, RigidForce, *RF, *Boundingbox, *BB, SkipUnit );
                
                                // Collision Response
                                if ( !SkipUnit )
                                    CheapaabbDynamic( Boundingbox, &RigidForce, Transform, m1, BB, RF, Xform, m2 );
                
                                // Update Collision State of Current Entity to Other Entity
                                Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = true;
                                Boundingbox->m_Collided = BB->m_Collided = true;
                            }
                
                            NotCollided = false;
                        }
                    }
                }
            });

            if ( NotCollided && Boundingbox )
            {
                for (auto& to_update : scripting_sys->scriptlist[Entity.m_GlobalIndex].m_Info)
                {
                    to_update.second->OnCollisionExit(Dynamic_Entity.m_GlobalIndex);
                }
                //BroadcastGlobalEvent<OnCollisionExit>(Entity, RigidForce, SkipUnit);
            }
        }
        else if ( Sphere )
        {
            ForEach( Search( SphereColliderQuery ), [&]( paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce& RF, sphere& Ball, mass* m2 ) noexcept
            {
                if ( SphereSphere( Transform.m_Position, Sphere->m_Radius, Xform.m_Position, Ball.m_Radius) )
                {
                    Sphere->m_Collided = Ball.m_Collided = true;
                }
                else
                    Sphere->m_Collided = Ball.m_Collided = false;
            });
        }
    }
};