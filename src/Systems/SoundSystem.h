#pragma once

struct sound_system : paperback::system::instance
{

    FMOD::System* m_p_SoundSystem = nullptr;

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "sound_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        // set up system aka fmod required stuff
        FMOD::System_Create(&m_p_SoundSystem);
        m_p_SoundSystem->init(32, FMOD_INIT_NORMAL, m_p_SoundSystem);
    }

    PPB_FORCEINLINE
    void Update( void ) noexcept
    {
        // call fmod default stuff IF there's something that needs to be "globally" called 
    }

    // entity that is processed by soundsystem will specifically have sound and timer components
    /*PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity, timer& timer, sound& sound) noexcept
    {
        // System Update Code - FOR A SINGLE ENTITY

    }*/

    //operator overload

    //dtor to clean system
    /*StopSound("All", true);

    // Terminate system
    m_p_SoundSystem->close();
    m_p_SoundSystem->release();*/

    //helper functions
    //add sound
    //delete sound
    //pause sound
    //mute sound


};