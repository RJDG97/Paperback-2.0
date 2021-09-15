#pragma once

struct sound
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "sound"
    };

	std::string m_SoundID;
    size_t m_SoundPlayTag; // assigned during run time to determine if currently playing
};