#pragma once

struct bullet_rendering_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "bullet_rendering_system"
    };

    using query = std::tuple<
        paperback::query::must<bullet>
    >;

    void operator()( transform& Transform, rigidbody& RigidBody )
    {
        constexpr auto ScaleX = 1 / 24.0f;
        constexpr auto ScaleY = 3 / 24.0f;
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(Transform.m_Position.m_X + RigidBody.m_Velocity.m_X * ScaleY, Transform.m_Position.m_Y + RigidBody.m_Velocity.m_Y * ScaleY);
        glVertex2i(Transform.m_Position.m_X + RigidBody.m_Velocity.m_Y * ScaleX, Transform.m_Position.m_Y - RigidBody.m_Velocity.m_X * ScaleX);
        glVertex2i(Transform.m_Position.m_X - RigidBody.m_Velocity.m_Y * ScaleX, Transform.m_Position.m_Y + RigidBody.m_Velocity.m_X * ScaleX);
        glEnd();
    }
};