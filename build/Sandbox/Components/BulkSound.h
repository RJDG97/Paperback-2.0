#pragma once

struct bulksound
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Bulk Sound"
    };

	std::string m_SoundPaths;       //contains string of paths concatenated by , 
    std::string m_SoundTags;        //used to assign a tag for identification, concatenated by , for each corresponding path
    bool m_IsTriggerable = true;    //set to true by default, assumes that sound is able to be played again whenever required
    bool m_Processed = false;       //set to false by default, used to determine if componnent has already been processed
    bool m_Is3D = false;            //set to false by default, used to determine if sounds are 3D sounds, applies to all sound events listed for this bulksound instance
};

namespace RR_BulkSound
{
    RTTR_REGISTRATION
    {
         rttr::registration::class_<bulksound>(bulksound::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Path", &bulksound::m_SoundPaths)
           .property("Tags", &bulksound::m_SoundTags)
           .property("Triggerable", &sound::m_IsTriggerable)(rttr::policy::prop::as_reference_wrapper)
           .property("Is 3D events", &bulksound::m_Is3D);
    }
}