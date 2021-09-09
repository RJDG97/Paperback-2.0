#pragma once

struct ship_rendering_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "ship_rendering_system"
    };

    using query = std::tuple<
        paperback::query::none_of<bullet>
    >;

    void operator()( transform& Transform, timer& Timer ) noexcept
    {
        constexpr auto Scale = 3;
        glBegin( GL_QUADS );
        if ( Timer.m_Timer > 0.0f ) glColor3f(1.0f, 1.0f, 1.0f);
        else                        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i( Transform.m_Position.m_X - Scale, Transform.m_Position.m_Y - Scale );
        glVertex2i( Transform.m_Position.m_X - Scale, Transform.m_Position.m_Y + Scale );
        glVertex2i( Transform.m_Position.m_X + Scale, Transform.m_Position.m_Y + Scale );
        glVertex2i( Transform.m_Position.m_X + Scale, Transform.m_Position.m_Y - Scale );
        glEnd();
    }
};