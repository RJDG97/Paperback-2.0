#pragma once
#include "Components/Transform.h"
#include "Components//Rigidbody.h"
#include "Components/Sound.h"
#include "Components/Timer.h"
#include "Systems/DebugSystem.h"

struct sound_system : paperback::system::pausable_instance
{

private:
    
    // used for storing in vector for keeping track of what song is curently playing
    struct SoundFile
    {
        FMOD::Studio::EventInstance* m_pSound = nullptr;        // contains pointer to playing sound
        size_t m_ID;                                            // contains the id to match with entity that spawned the sound
        bool m_IsTriggerable;                                   // clone of variable in component for deciding if entity is to be purged on sound play completion
        bool m_Trigger = false;                                 // clone of variable in sound component, used particularly with sounds not set by sound component 
        bool m_Verified;                                        // used to verify if a soundfile being played has a corresponding active sound component
        bool m_ForceStop = false;                               // used to force set a sound file to stop 
        bool m_IsStandalone = false;                            // used to to tell if a sound file is paired to a sound component or is set by standalone 
        std::string m_Tag;
    };

public:
    
    using query = std::tuple
    <
        paperback::query::must<sound>
    ,   paperback::query::one_of<transform, rigidbody, listener>
    ,   paperback::query::none_of<prefab>
    >;

    using alt_query = std::tuple
    <
        paperback::query::must<bulksound>
    ,   paperback::query::none_of<prefab>
    >;

    tools::query m_SoundQuery, m_BulkSoundQuery;

    FMOD::System* m_pFMODSystem = nullptr; // contains pointer to fmod system
    FMOD::Studio::System* m_pStudioSystem = nullptr;
    std::vector<SoundFile> m_SoundFiles;
    size_t m_SoundCounter;
    paperback::Vector3f m_ListenerPos, m_ListenerVel;
    bool m_AudioFollowPosition;


    //helper functions

    //add sound bank
    // helper function
    // adds a sound bank
    // bank contains event and metadata info for triggering sound events
    // audio data not included, but can be loaded either all in one go or on event call
    void AddBank(const char* Filename) 
    {

        FMOD::Studio::Bank* result;
        FMOD_RESULT res = m_pStudioSystem->loadBankFile(Filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &result);

        if (!result) 
        {

            ERROR_LOG("Unable to load Sound Bank: '" + std::string{ Filename } + "' with result: " + std::to_string(res));
        }
        else
        {
            
            ERROR_LOG("Sound Bank: '" + std::string{ Filename } + "' successfullly loaded");
        }
    }

    void RemoveAllBanks() 
    {

        m_pStudioSystem->unloadAll();
    }

    //play event 
    // helper function
    // loads and plays an event from the current loaded bank
    void PlaySoundEvent( const std::string_view& Path, bool IsTriggerable = false ) 
    {

        FMOD::Studio::EventDescription* event = nullptr;
        
        if (m_pStudioSystem->getEvent( Path.data(), &event) == FMOD_OK ) 
        {

            m_SoundFiles.push_back({});
            m_SoundFiles.back().m_ID = ++m_SoundCounter;

            event->createInstance(&m_SoundFiles.back().m_pSound);
            m_SoundFiles.back().m_IsTriggerable = IsTriggerable;

            if (!IsTriggerable)
            {

                FMOD_RESULT result = m_SoundFiles.back().m_pSound->start();

                ERROR_LOG("Play Sound Event Result: " + result);

                //in case of extra load case to debug log
                FMOD_STUDIO_PLAYBACK_STATE be;
                m_SoundFiles.back().m_pSound->getPlaybackState(&be);

                m_SoundFiles.back().m_pSound->setVolume(0.1f);

                if (be != 0)
                    ERROR_LOG("Play Sound Event Playback State: " + be);
            }
        }
        else {

            ERROR_LOG("Sound Event: '" + std::string(Path)  + "' does not exist in current Sound Bank");
        }
    }

    void StopTriggeredSoundEvent(size_t SoundTag)
    {

        auto sound_check = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [SoundTag](const SoundFile& soundfile) { return SoundTag == soundfile.m_ID; });

        if (sound_check != m_SoundFiles.end())
            sound_check->m_ForceStop = true;
    }

    //variation for targetting sound events that were added by bulk sound components
    void StopTriggeredSoundEvent(const std::string& Tag)
    {

        for (size_t i = 0; i < m_SoundFiles.size(); ++i)
        {

            //only process sounds that are tagged and added through bulk sound component
            if (!m_SoundFiles[i].m_IsStandalone && m_SoundFiles[i].m_Tag != Tag)
                continue;

            m_SoundFiles[i].m_ForceStop = true;
        }
    }

    //stop sound
    // helper function
    // stops sound from currently playing
    void StopSoundEvent(FMOD::Studio::EventInstance* Instance) 
    {

        Instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        Instance->release();
    }

    //stop all sound
    // helper function
    // stops all sound events
    void StopAllSounds()
    {
        for (SoundFile& sound : m_SoundFiles)
        {

            sound.m_IsTriggerable = false;
            StopSoundEvent(sound.m_pSound);
        }
    }


    //pause sound
    // helper function
    // pauses/unpauses sound channels
    void PauseSoundEvent(FMOD::Studio::EventInstance* Instance)
    {
        bool paused_status;
        Instance->getPaused(&paused_status);

        if (paused_status)
        {
            Instance->setPaused(true);
        }
        else
        {
            Instance->setPaused(false);
        }
    }

    //convert transform & vel to fmod 3d attribute
    //helper function
    //to modify when 3d switch
    void ConvertSystemToFMOD3D(FMOD_3D_ATTRIBUTES& Attribute, const paperback::Vector3f& Position, const paperback::Vector3f& Velocity)
    {
        //2d now, to change to 3d when shifted to 3d
        paperback::Vector3f normal_vec = Velocity;
        normal_vec.Normalized();

        float zerocheck = normal_vec.MagnitudeFast();

        if (zerocheck > 0.01f && zerocheck < -0.01f)
        {

            Attribute.forward.x = normal_vec.x;
            Attribute.forward.y = normal_vec.y;
            Attribute.forward.z = normal_vec.z;
        }
        else
        {

            Attribute.forward.z = 1.0f;
        }

        //needs to be perpendicular to forward vec
        Attribute.up.y = 1.0f;

        Attribute.position.x = Position.x;
        Attribute.position.y = Position.y;
        Attribute.position.z = Position.z;

        Attribute.velocity.x = Velocity.x;
        Attribute.velocity.y = Velocity.y;
        Attribute.velocity.z = Velocity.z;
    }

    //set player 3d attributes
    // helper function
    // sets the player's FMOD 3d attributes for 3D sound
    void UpdatePlayer3DAttributes(const paperback::Vector3f& Position, const paperback::Vector3f& Velocity)
    {

        FMOD_3D_ATTRIBUTES attribute{};

        ConvertSystemToFMOD3D(attribute, Position, Velocity);

        m_pStudioSystem->setListenerAttributes(0, &attribute);
    }

    //set 3D attributes
    // helper function
    // sets a sound instance's 3d attributes
    void UpdateEvent3DAttributes(FMOD::Studio::EventInstance* Instance, const transform* Transform, const rigidbody* Rigidbody)
    {

        FMOD_3D_ATTRIBUTES attribute{};
        ConvertSystemToFMOD3D(attribute, Transform->m_Position, Rigidbody->m_Velocity);

        Instance->set3DAttributes(&attribute);
    }

    //pause/unpause all sounds
    // helper function
    // pauses/unpauses all currently playing sounds
    void PauseCurrentSounds(const bool PauseStatus)
    {

        for (SoundFile& sound : m_SoundFiles) {

            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);

            //if sound is not stopped, set pause status
            if (be != 2) {

                sound.m_pSound->setPaused(PauseStatus);
            }
        }
    }

    //Change Event Parameter
    // helper function
    // changes the value of a currently playing event
    // ideally has a separate logic calling this when required
    void ChangeEventParameters(FMOD::Studio::EventInstance* Instance, const char* ParamName, const float Value)
    {

        Instance->setParameterByName(ParamName, Value);
    }


    //Debug test bank loader
    // helper function
    // used for debug loading of a test bank
    void LoadDebugBank()
    {

        AddBank("../../resources/soundbank/Master.bank");
        AddBank("../../resources/soundbank/Master.strings.bank");
    }

    //function to decide what primary listener location is based on 
    // either preset (currently global center) or object position with a specific component
    void SetListenerPosition()
    {

        if (m_AudioFollowPosition)
        {

            UpdatePlayer3DAttributes(m_ListenerPos, m_ListenerVel);
            m_AudioFollowPosition = false;
        }
        else
        {

            UpdatePlayer3DAttributes({}, {});
        }
    }


    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "sound_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        // set up system aka fmod required stuff
        
        //create fmod studio system instance
        FMOD::Studio::System::create(&m_pStudioSystem);
        m_pStudioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);

        // store pointer to underlying fmod system
        m_pStudioSystem->getCoreSystem(&m_pFMODSystem);
        
        LoadDebugBank();

        m_SoundCounter = {};

        m_pStudioSystem->setNumListeners(1);

        m_AudioFollowPosition = false;

        m_ListenerPos = {};
        m_ListenerVel = {};

        m_SoundQuery.AddQueryFromTuple(xcore::types::null_tuple_v< query >);
        m_BulkSoundQuery.AddQueryFromTuple(xcore::types::null_tuple_v< alt_query >);
    }


    void SoundComponentProcess()
    {

        ForEach(Search(m_SoundQuery), [&](entity& Entity, sound& Sound, transform* Transform, rigidbody* Rigidbody, listener* Listener) noexcept
        {

            if (Entity.IsZombie())
                return;

            //for updating current central listener
            if (Sound.m_SoundID == "")
            {

                if (Listener)
                {

                    m_AudioFollowPosition = true;

                    if (Transform)
                        m_ListenerPos = Transform->m_Position;

                    if (Rigidbody)
                        m_ListenerVel = Rigidbody->m_Velocity;
                    else
                        m_ListenerVel.z = 1.0f;
                }

                return;
            }

            auto sound_check = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });
            //check if id already exists 
            if (sound_check == m_SoundFiles.end())
            {

                //if no, then create new entry and add into record of currently playing sounds
                PlaySoundEvent(Sound.m_SoundID, Sound.m_IsTriggerable);
                Sound.m_SoundPlayTag = m_SoundCounter;

                if (!m_SoundFiles.empty())
                    m_SoundFiles.back().m_Verified = true;
            }
            else if (Sound.m_IsTriggerable)
            {

                //sound exists and is triggerable
                //check current playback status

                sound_check->m_Verified = true;
                FMOD_STUDIO_PLAYBACK_STATE be;
                sound_check->m_pSound->getPlaybackState(&be);


                //if sound is stopped or yet to begin, check trigger status
                if (be == FMOD_STUDIO_PLAYBACK_STOPPED && Sound.m_Trigger)
                {

                    //trigger is active, play the sound and reset trigger
                    Sound.m_Trigger = false;
                    sound_check->m_pSound->start();
                }
                if (Sound.m_ForceStop)
                {
                    if (be == FMOD_STUDIO_PLAYBACK_STOPPED)
                    {

                        //not triggered but force stop
                        //reset
                        Sound.m_ForceStop = false;
                    }
                    else //sound_check->m_ForceStop)
                    {

                        //sound_check->m_ForceStop = false;
                        Sound.m_ForceStop = false;
                        sound_check->m_pSound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                    }
                }
            }
            else
            {

                //sound exists, ensure that id is not 0, else delete
                FMOD_STUDIO_PLAYBACK_STATE be;
                sound_check->m_pSound->getPlaybackState(&be);

                //if sound has stopped, mark for removal
                if (be == 2)
                {
                    sound_check->m_ID = 0;
                    return;
                }

                sound_check->m_Verified = true;
            }


            //process sound based on position of listener
            if (Sound.m_Is3DSound && Transform && Rigidbody)
            {

                //file should exist, no need to error check
                auto soundfile = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });

                UpdateEvent3DAttributes(soundfile->m_pSound, Transform, Rigidbody);
            }
        });
    }

    void ProcessPathsOrTags(std::vector<std::string>& paths, const std::string& unformat)
    {

        std::string path{};
        std::stringstream format{ unformat };

        while (std::getline(format, path, ','))
        {

            paths.push_back(path);
        }
    }

    void BulkSoundProcess()
    {

        //process bulk sound components here

        ForEach(Search(m_BulkSoundQuery), [&](entity& Entity, bulksound& Bulk) noexcept
        {

                if (Entity.IsZombie() || Bulk.m_Processed || Bulk.m_SoundPaths == "" || Bulk.m_SoundTags == "")
                    return;

                //assume there is the same number of paths and tags
                std::vector<std::string> paths, tags;

                //split from commas into separate pairs of paths and tags
                ProcessPathsOrTags(paths, Bulk.m_SoundPaths);
                ProcessPathsOrTags(tags, Bulk.m_SoundTags);

                //create sound events for each that exist, assume that same number of tags and sounds

                for (size_t i = 0; i < paths.size(); ++i)
                {
                    PlaySoundEvent(paths[i], Bulk.m_IsTriggerable);

                    if (!m_SoundFiles.empty())
                    {

                        m_SoundFiles.back().m_Verified = true;
                        m_SoundFiles.back().m_IsStandalone = true;
                        m_SoundFiles.back().m_Tag = tags[i];
                    }
                }

                Bulk.m_Processed = true;
        });

        //process indep sound components
        for (SoundFile& sound : m_SoundFiles) {

            sound.m_Verified = true;
            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);


            //if sound is stopped or yet to begin, check trigger status
            if (be == FMOD_STUDIO_PLAYBACK_STOPPED && sound.m_Trigger)
            {

                //trigger is active, play the sound and reset trigger
                sound.m_Trigger = false;
                sound.m_pSound->start();
            }
            if (sound.m_ForceStop)
            {
                if (be == FMOD_STUDIO_PLAYBACK_STOPPED)
                {

                    //not triggered but force stop
                    //reset
                    sound.m_ForceStop = false;
                }
                else //sound_check->m_ForceStop)
                {

                    //sound_check->m_ForceStop = false;
                    sound.m_ForceStop = false;
                    sound.m_pSound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                }
            }
        }
    }


    // entity that is processed by soundsystem will specifically have sound and timer components
    // entity must have either transform or rigidbody, can have both if is 3D
    PPB_FORCEINLINE
    void Update() noexcept
    {

        SoundComponentProcess();
        BulkSoundProcess();
    }

    /*void operator()(paperback::component::entity& Entity, sound& Sound, transform* Transform, rigidbody* Rigidbody, listener* Listener) noexcept
    {
        if ( Entity.IsZombie() )
            return;

        //for updating current central listener
        if (Sound.m_SoundID == "")
        {

            if (Listener)
            {

                m_AudioFollowPosition = true;

                if (Transform)
                    m_ListenerPos = Transform->m_Position;

                if (Rigidbody)
                    m_ListenerVel = Rigidbody->m_Velocity;
                else
                    m_ListenerVel.z = 1.0f;
            }

            return;
        }

        auto sound_check = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });
        //check if id already exists 
        if (sound_check == m_SoundFiles.end())
        {

            //if no, then create new entry and add into record of currently playing sounds
            PlaySoundEvent( Sound.m_SoundID, Sound.m_IsTriggerable );
            Sound.m_SoundPlayTag = m_SoundCounter;

            if (!m_SoundFiles.empty())
                m_SoundFiles.back().m_Verified = true;
        }
        else if (Sound.m_IsTriggerable)
        {

            //sound exists and is triggerable
            //check current playback status

            sound_check->m_Verified = true;
            FMOD_STUDIO_PLAYBACK_STATE be;
            sound_check->m_pSound->getPlaybackState(&be);
                

            //if sound is stopped or yet to begin, check trigger status
            if (be == FMOD_STUDIO_PLAYBACK_STOPPED && Sound.m_Trigger)
            {

                //trigger is active, play the sound and reset trigger
                Sound.m_Trigger = false;
                sound_check->m_pSound->start();
            }
            if (Sound.m_ForceStop)
            {
                if (be == FMOD_STUDIO_PLAYBACK_STOPPED)
                {

                    //not triggered but force stop
                    //reset
                    Sound.m_ForceStop = false;
                }
                else //sound_check->m_ForceStop)
                {

                    //sound_check->m_ForceStop = false;
                    Sound.m_ForceStop = false;
                    sound_check->m_pSound->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                }
            }
        }
        else
        {

            //sound exists, ensure that id is not 0, else delete
            FMOD_STUDIO_PLAYBACK_STATE be;
            sound_check->m_pSound->getPlaybackState(&be);

            //if sound has stopped, mark for removal
            if (be == 2)
            {
                sound_check->m_ID = 0;
                return;
            }

            sound_check->m_Verified = true;
        }


        //process sound based on position of listener
        if (Sound.m_Is3DSound && Transform && Rigidbody)
        {

            //file should exist, no need to error check
            auto soundfile = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });
            
            UpdateEvent3DAttributes(soundfile->m_pSound, Transform, Rigidbody);
        }
    }*/

    PPB_FORCEINLINE
    void CleanUpSounds()
    {

        //set sound file for deletion if not verified to have corresponding sound component or has stopped and is not triggerable
        for (SoundFile& sound : m_SoundFiles) {

            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);

            //if sound has stopped, mark for removal
            if ((be == 2 && !sound.m_IsTriggerable) || !sound.m_Verified) {

                sound.m_ID = 0;
            }
        }

        //remove all sound files tagged with id 0 since 0 is default tag value which should have been replaced with non-zero from start
        auto end = std::remove_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [](SoundFile& sound) { return (sound.m_ID == 0); });

        if (end != std::end(m_SoundFiles))
            m_SoundFiles.erase(end, m_SoundFiles.end());

        //reset verification status for next loop
        for (SoundFile& sound : m_SoundFiles) {

            sound.m_Verified = false;
        }

        // call fmod default stuff IF there's something that needs to be "globally" called 
        m_pStudioSystem->update();
    }


    PPB_FORCEINLINE
    void OnFrameEnd(void) noexcept 
    {

        CleanUpSounds();

        //update listener position
        SetListenerPosition();
    }

    // Terminate system
    PPB_FORCEINLINE
    void OnSystemTerminated(void) noexcept 
    {

        for (SoundFile& sound : m_SoundFiles) {

            StopSoundEvent(sound.m_pSound);
            sound.m_Verified = false;
        }

        m_SoundFiles.clear();

        m_pStudioSystem->unloadAll();
        m_pStudioSystem->release();
    }

    PPB_FORCEINLINE
    void OnStateChange(void) noexcept
    {

        StopAllSounds();
    }

    PPB_FORCEINLINE
    void EditorStopAllSounds()
    {

        StopAllSounds();
        CleanUpSounds();
    };

    //used to play a stand alone sound attached to a specific tag
    void TriggerTaggedSound(const std::string& Tag)
    {

        auto sound_check = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Tag](const SoundFile& soundfile) { return Tag == soundfile.m_Tag; });

        if (sound_check != m_SoundFiles.end())
        {

            sound_check->m_pSound->start();
        }
    }

    //used to play one of multiple sounds attached to a specific tag
    //useful for multiple sounds for the same action
    //e.g. footsteps
    void TriggerOneOfGroupTaggedSounds(const std::string& Tag)
    {

        std::vector<size_t> indexes{};

        for (size_t i = 0; i < m_SoundFiles.size(); ++i)
        {

            //only process sounds that are tagged and added through bulk sound component
            if (!m_SoundFiles[i].m_IsStandalone && m_SoundFiles[i].m_Tag != Tag)
                continue;

            //check if sound file status is not playing, otherwise in progress, cut search and return
            FMOD_STUDIO_PLAYBACK_STATE be;
            m_SoundFiles[i].m_pSound->getPlaybackState(&be);

            if (be == FMOD_STUDIO_PLAYBACK_PLAYING || be == FMOD_STUDIO_PLAYBACK_STARTING)
                return;

            indexes.push_back(i);
        }        

        //randomly select one of available indexes to 
        if (!indexes.empty())
            m_SoundFiles[indexes[rand() % indexes.size()]].m_pSound->start();
    }
};