#pragma once
#include "Components/Transform.h"
#include "Components//Rigidbody.h"
#include "Components/Sound.h"
#include "Components/Timer.h"
#include "Systems/DebugSystem.h"

struct sound_system : paperback::system::instance
{

private:
    
    // used for storing in vector for keeping track of what song is curently playing
    struct SoundFile
    {
        FMOD::Studio::EventInstance* m_pSound = nullptr; // contains pointer to playing sound
        size_t m_ID; // contains the id to match with entity that spawned the sound
    };

public:

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
    void PlaySoundEvent( const std::string_view& Path) 
    {

        FMOD::Studio::EventDescription* event = nullptr;
        
        if (m_pStudioSystem->getEvent( Path.data(), &event) == FMOD_OK ) 
        {

            m_SoundFiles.push_back({});
            m_SoundFiles.back().m_ID = ++m_SoundCounter;

            event->createInstance(&m_SoundFiles.back().m_pSound);
            FMOD_RESULT result = m_SoundFiles.back().m_pSound->start(); 
            
            ERROR_LOG("Play Sound Event Result: " + result);

            //in case of extra load case to debug log
            FMOD_STUDIO_PLAYBACK_STATE be;
            m_SoundFiles.back().m_pSound->getPlaybackState(&be);

            m_SoundFiles.back().m_pSound->setVolume(0.05f);

            if (be != 0)
                ERROR_LOG("Play Sound Event Playback State: " + be);
        }
        else {

            ERROR_LOG("Sound Event: '" + std::string(Path)  + "' does not exist in current Sound Bank");
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
    }

    // entity that is processed by soundsystem will specifically have sound and timer components
    // entity must have either transform or rigidbody, can have both if is 3D
    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, timer& Timer, sound& Sound, transform* Transform, rigidbody* Rigidbody, listener* Listener) noexcept
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
            PlaySoundEvent( Sound.m_SoundID );
            Sound.m_SoundPlayTag = m_SoundCounter;
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
        }


        //process sound based on position of listener
        if (Sound.m_Is3DSound && Transform && Rigidbody)
        {

            //file should exist, no need to error check
            auto soundfile = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });
            
            UpdateEvent3DAttributes(soundfile->m_pSound, Transform, Rigidbody);
        }
    }


    PPB_FORCEINLINE
    void OnFrameEnd(void) noexcept 
    {

        // call fmod default stuff IF there's something that needs to be "globally" called 
        m_pStudioSystem->update();

        for (SoundFile& sound : m_SoundFiles) {

            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);

            //if sound has stopped, mark for removal
            if (be == 2) {

                sound.m_ID = 0;
            }
        }

        //remove all sound files tagged with id 0 since 0 is default tag value which should have been replaced with non-zero from start
        auto end = std::remove_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [](SoundFile& sound) { return sound.m_ID == 0; });

        if (end != std::end(m_SoundFiles))
            m_SoundFiles.erase(end, m_SoundFiles.end());

        //update listener position
        SetListenerPosition();
    }

    // Terminate system
    PPB_FORCEINLINE
    void OnSystemTerminated(void) noexcept 
    {

        for (SoundFile& sound : m_SoundFiles) {

            StopSoundEvent(sound.m_pSound);
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
};