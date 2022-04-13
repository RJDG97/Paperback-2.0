/**********************************************************************************
*\file         CollisionSystem.h
*\brief        CollisionSystem.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Physics/geometry.h"
#include "Systems/DebugSystem.h"
#include "Systems/ScriptingSystem.h"
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
    tools::query CollidableQuery;
    tools::query m_InteractableQuery;
    tools::query m_Query;
    scripting_system* scripting_sys;


    //test
    paperback::u32 m_PushID;

    PPB_INLINE
	void OnStateChange( void ) noexcept
	{
        m_PushID = paperback::settings::invalid_index_v;
	}


	PPB_INLINE
	void OnStateLoad( void ) noexcept
	{
        ForEach( Search( m_InteractableQuery ), [&]( entity& PushEntity )
        {
            m_PushID = PushEntity.m_GlobalIndex;
        });
	}


    PPB_FORCEINLINE
	void OnSystemCreated( void ) noexcept
	{
        scripting_sys = &GetSystem<scripting_system>();
		SphereColliderQuery.m_Must.AddFromComponents < transform, sphere, rigidforce >();
        SphereColliderQuery.m_OneOf.AddFromComponents< mass, bounding_volume >();
        SphereColliderQuery.m_NoneOf.AddFromComponents< prefab >();

        CollidableQuery.m_Must.AddFromComponents < transform>();
        CollidableQuery.m_OneOf.AddFromComponents< rigidforce, boundingbox, mass, slope, bounding_volume >();
        CollidableQuery.m_NoneOf.AddFromComponents< prefab >();

        m_Query.m_Must.AddFromComponents<paperback::component::entity, transform, rigidforce>();
        m_Query.m_OneOf.AddFromComponents< name, boundingbox, sphere, mass, slope, bounding_volume >();
        m_Query.m_NoneOf.AddFromComponents<prefab>();

        m_InteractableQuery.m_Must.AddFromComponents<paperback::component::entity, transform, rigidforce, player_interaction>();
        m_InteractableQuery.m_OneOf.AddFromComponents< name, boundingbox, sphere, mass, slope, bounding_volume >();
        m_InteractableQuery.m_NoneOf.AddFromComponents<prefab>();
	}

    PPB_FORCEINLINE
    //void PreUpdate(void) noexcept
    void Update(void) noexcept
    {
        ForEach( Search( m_Query ), [&]( paperback::component::entity& Entity, transform& Transform, rigidforce& RigidForce, boundingbox* Boundingbox
                                       , sphere* Sphere, mass* m1, slope* Slope1, bounding_volume* BV ) noexcept
        {
            // Exclude Deleted Entities & Static Objects that are NOT Bounding Volumes
            if ( Entity.IsZombie() || ( !BV && m1 && m1->m_Mass == 0.0f ) ) return;

            if ( Boundingbox )
            {
                Boundingbox->m_Collided = false;

                auto NeighbourList = m_Coordinator.QueryNeighbours( *Boundingbox, Transform, BV ? 2.0f : 0.0f );

                ForEach( NeighbourList, [&]( entity& Dynamic_Entity, transform& Xform, rigidforce* RF, boundingbox* BB, mass* m2, slope* Slope2, bounding_volume* BV2, pushable* Pushable )
                {
                    if (Entity.IsZombie() || Dynamic_Entity.IsZombie()) return;

                    // Do not check against self
                    if (&Entity != &Dynamic_Entity)
                    {
                        // Add to collision map
                        auto map = Boundingbox->m_CollisionState.find(Dynamic_Entity.m_GlobalIndex);
                        if (map == Boundingbox->m_CollisionState.end()) {
                            const auto& [map, Valid] = Boundingbox->m_CollisionState.insert({ Dynamic_Entity.m_GlobalIndex, false });
                        }

                        // Collision Detection
                        if (Boundingbox && BB)
                        {
                            // If Both Entities Are Colliding Already
                            if (AabbAabb(Transform.m_Position + Transform.m_Offset + Boundingbox->Min, Transform.m_Position + Transform.m_Offset + Boundingbox->Max
                                , Xform.m_Position + Xform.m_Offset + BB->Min, Xform.m_Position + Xform.m_Offset + BB->Max))
                            {
                                if (RF)
                                {
                                    // Current Entity is NOT Colliding with Other Entity
                                    if (!Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex)) {

                                            //m_Coordinator.SetTotalProcesses(1);
                                            //m_Coordinator.SetProcessesCompleted(0);

                                        auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                        if (entry_found == scripting_sys->scriptlist.end()) {

                                            entry_found->second->OnCollisionEnter(Dynamic_Entity.m_GlobalIndex);
                                        }

                                        if (!(BV || BV2))
                                        {
                                            //m_Coordinator.SetTotalProcesses(1);
                                            //m_Coordinator.SetProcessesCompleted(0);
                                            auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                            if (entry_found == scripting_sys->scriptlist.end()) {

                                                entry_found->second->OnCollisionEnter(Entity.m_GlobalIndex);
                                            }

                                            //while (!m_Coordinator.CompareProcesses())
                                            //{
                                            //}
                                        }

                                    }
                                    // Current Entity is ALREADY Colliding with Other Entity
                                    else
                                    {
                                        //m_Coordinator.SetTotalProcesses(1);
                                        //m_Coordinator.SetProcessesCompleted(0);

                                        auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                        if (entry_found == scripting_sys->scriptlist.end()) {

                                            entry_found->second->OnCollisionStay(Dynamic_Entity.m_GlobalIndex);
                                        }

                                        //while (!m_Coordinator.CompareProcesses())
                                        //{
                                        //}

                                        if (!(BV || BV2))
                                        {
                                            /*m_Coordinator.SetTotalProcesses(1);
                                            m_Coordinator.SetProcessesCompleted(0);*/

                                            auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                            if (entry_found == scripting_sys->scriptlist.end()) {

                                                entry_found->second->OnCollisionStay(Entity.m_GlobalIndex);
                                            }


                                            //while (!m_Coordinator.CompareProcesses())
                                            //{
                                            //}
                                        }
                                    }

                                    // Collision Response If Not Bounding Volume
                                    if (!(BV || BV2))
                                    {
                                        if ( Entity.m_GlobalIndex == m_PushID && Pushable )
                                        {
                                            rigidforce static_rf{};
                                            mass       static_mass{};
                                            AABBDynamic( Boundingbox, &RigidForce, Transform, m1, Slope1
                                                       , BB, &static_rf, Xform, &static_mass, Slope2 );
                                        }
                                        else
                                        {
                                            AABBDynamic( Boundingbox, &RigidForce, Transform, m1, Slope1
                                                       , BB, RF, Xform, m2, Slope2 );
                                        }
                                    }

                                    // Update Collision State of Current Entity to Other Entity
                                    Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = true;
                                    Boundingbox->m_Collided = BB->m_Collided = true;
                                }
                            }

                            //Current Entity is Colliding with Other Entity in the prev frame
                            else if (Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex))
                            {
                                /*m_Coordinator.SetTotalProcesses(1);
                                m_Coordinator.SetProcessesCompleted(0);*/

                                auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                if (entry_found == scripting_sys->scriptlist.end()) {

                                    entry_found->second->OnCollisionExit(Dynamic_Entity.m_GlobalIndex);
                                }

                                //while (!m_Coordinator.CompareProcesses())
                                //{
                                //}

                                if (!(BV || BV2))
                                {
                                    /*m_Coordinator.SetTotalProcesses(1);
                                    m_Coordinator.SetProcessesCompleted(0);*/
                                    auto entry_found = scripting_sys->scriptlist.find(Dynamic_Entity.m_GlobalIndex);

                                    if (entry_found == scripting_sys->scriptlist.end()) {

                                        entry_found->second->OnCollisionExit(Entity.m_GlobalIndex);

                                    //while (!m_Coordinator.CompareProcesses())
                                    //{
                                    //}
                                }

                                Boundingbox->m_CollisionState.at(Dynamic_Entity.m_GlobalIndex) = false;
                            }
                        }
                    }
                });
            }

            else if (Sphere)
            {
                ForEach(Search(SphereColliderQuery), [&](paperback::component::entity& Dynamic_Entity, transform& Xform, rigidforce& RF, sphere& Ball, mass* m2) noexcept
                    {
                        if (SphereSphere(Transform.m_Position + Transform.m_Offset, Sphere->m_Radius, Xform.m_Position + Xform.m_Offset, Ball.m_Radius))
                        {
                            Sphere->m_Collided = Ball.m_Collided = true;
                        }
                        else
                            Sphere->m_Collided = Ball.m_Collided = false;
                    });
            }
        });
    }


    /*PPB_FORCEINLINE
    void Update(void) noexcept
    {

    }*/
};