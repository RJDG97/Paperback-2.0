#pragma once

struct sound
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Sound"
    };

	std::string m_SoundID; //contains the path/id of the event to be played from the bank
    size_t m_SoundPlayTag; // assigned during run time to determine if currently playing
    bool m_Is3DSound; //set to false if 2D, true if 3D
};