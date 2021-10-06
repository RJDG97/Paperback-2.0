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

    void operator()( transform& Transform, rigidbody& RigidBody ) noexcept
    {
        Transform.m_Position += RigidBody.m_Velocity * m_Coordinator.DeltaTime();
    }
};