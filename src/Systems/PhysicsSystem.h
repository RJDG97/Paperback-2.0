#pragma once

struct physics_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "physics_system"
    };

    int DebugTest = 0;

    using query = std::tuple<
        paperback::query::one_of<>
    >;

    void operator()( transform& Transform, rigidbody& RigidBody, paperback::component::entity* Entity, bullet* BulletTest ) noexcept
    {
        Transform.m_Position += RigidBody.m_Velocity * m_Coordinator.DeltaTime();

        /*
            Remove if unnecessary - Add Remove Component Reference
            1st Tuple = Add
            2nd Tuple = Remove
        */
        {
            //// Remove Bullet Component Test
            //if ( !BulletTest )
            //    TRACE_PRINT( "Component removed successfully" );

            //if ( BulletTest && Entity )
            //{
            //    TRACE_PRINT( "Entity Global Index: " + std::to_string(Entity->m_GlobalIndex) );

            //    auto UpdatedEntityComponent = m_Coordinator.AddOrRemoveComponents< std::tuple<>,                          // Tuple of Components to Add
            //                                                                       std::tuple<bullet> >( *Entity );       // Tuple of Components to Remove

            //    INFO_PRINT( "Removing bullet component from entity in Physics System" );
            //    return;
            //}
            

            //// Add Bullet Component Test
            //if ( BulletTest )
            //    TRACE_PRINT( "Component added successfully - ID: " + std::to_string( Entity->m_GlobalIndex ) );

            //if ( !BulletTest && Entity )
            //{
            //    TRACE_PRINT( "Entity Global Index: " + std::to_string(Entity->m_GlobalIndex) );

            //    auto UpdatedEntityComponent = m_Coordinator.AddOrRemoveComponents< std::tuple<bullet> >( *Entity );

            //    INFO_PRINT( "Adding bullet component to entity in Physics System" );
            //    return;
            //}
        }

        // X-Out-Of-Bounds
        if (Transform.m_Position.m_X < 0.0f)
        {
            Transform.m_Position.m_X = 0.0f;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }
        else if (Transform.m_Position.m_X >= m_Engine.m_Width)
        {
            Transform.m_Position.m_X = m_Engine.m_Width;
            RigidBody.m_Velocity.m_X = -RigidBody.m_Velocity.m_X;
        }

        // Y-Out-Of-Bounds
        if (Transform.m_Position.m_Y < 0.0f)
        {
            Transform.m_Position.m_Y = 0.0f;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }
        else if (Transform.m_Position.m_Y >= m_Engine.m_Height)
        {
            Transform.m_Position.m_Y = m_Engine.m_Height;
            RigidBody.m_Velocity.m_Y = -RigidBody.m_Velocity.m_Y;
        }
    }
};