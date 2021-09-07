#pragma once

struct buffer_refresh_system : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "buffer_refresh_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        
    }

    PPB_FORCEINLINE
    void Update( void )
    {
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT);
    }
};