#pragma once
struct imgui_system : paperback::system::instance 
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "imgui_system"
    };

    //Handle Imgui Initialization Here
    PPB_INLINE
    void OnSystemCreated ( void ) noexcept {}

    //Handle Imgui Main Loop Here (For all the windows)
    PPB_INLINE
    void Update( void ){}

    PPB_INLINE
    void OnFrameStart( void ) noexcept {}

    PPB_INLINE
    void OnFrameEnd( void ) noexcept {}
};