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
    void PlaySoundEvent(const std::string_view& PathView) 
    {

        FMOD::Studio::EventDescription* event = nullptr;
        
        if (m_pStudioSystem->getEvent(PathView.data(), &event) == FMOD_OK) 
        {

            m_SoundFiles.push_back({});
            m_SoundFiles.back().m_ID = ++m_SoundCounter;

            //FMOD::Studio::EventInstance* instance = nullptr;
            //event->loadSampleData();
            event->createInstance(&m_SoundFiles.back().m_pSound);
            FMOD_RESULT result = m_SoundFiles.back().m_pSound->start(); 
            
            ERROR_LOG("Play Sound Event Result: " + result);

            m_SoundFiles.back().m_pSound->setVolume(0.5f);

            //in case of extra load case to debug log
            FMOD_STUDIO_PLAYBACK_STATE be;
            m_SoundFiles.back().m_pSound->getPlaybackState(&be);

            if (be != 0)
                ERROR_LOG("Play Sound Event Playback State: " + be);
        }
        else {

            ERROR_LOG("Sound Event: '" + std::string{ PathView.data() }  + "' does not exist in current Sound Bank");
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
    void ConvertSystemToFMOD3D(FMOD_3D_ATTRIBUTES& Attribute, const transform* Transform, const rigidbody* Rigidbody)
    {
        //2d now, to change to 3d when shifted to 3d
        paperback::Vector3f normal_vec = Rigidbody->m_Velocity;
        normal_vec.Normal2D();

        Attribute.forward.x = normal_vec.x;
        Attribute.forward.y = normal_vec.y;

        //attribute.up.z = 0.0f;

        Attribute.position.x = Transform->m_Position.x;
        Attribute.position.y = Transform->m_Position.y;

        Attribute.velocity.x = Rigidbody->m_Velocity.x;
        Attribute.velocity.y = Rigidbody->m_Velocity.y;
    }

    //set player 3d attributes
    // helper function
    // sets the player's FMOD 3d attributes for 3D sound
    void UpdatePlayer3DAttributes(const transform* Transform, const rigidbody* Rigidbody) 
    {

        FMOD_3D_ATTRIBUTES attribute{};
        ConvertSystemToFMOD3D(attribute, Transform, Rigidbody);

        m_pStudioSystem->setListenerAttributes(0, &attribute);
    }

    //set 3D attributes
    // helper function
    // sets a sound instance's 3d attributes
    void UpdateEvent3DAttributes(FMOD::Studio::EventInstance* Instance, const transform* Transform, const rigidbody* Rigidbody)
    {

        FMOD_3D_ATTRIBUTES attribute{};
        ConvertSystemToFMOD3D(attribute, Transform, Rigidbody);

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

        AddBank("TestBank/Master.bank");
        AddBank("TestBank/Master.strings.bank");
        /*AddBank("TestBank/Dialogue_EN.bank");
        AddBank("TestBank/Music.bank");
        AddBank("TestBank/SFX.bank");
        AddBank("TestBank/Vehicles.bank");
        AddBank("TestBank/VO.bank");
        */
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

        //add master bank file
        //AddBank("Master.bank");
        //AddBank("Master.strings.bank");
        
        LoadDebugBank();

        m_SoundCounter = {};

        m_pStudioSystem->setNumListeners(1);
    }

    PPB_FORCEINLINE
    void OnFrameStart(void) noexcept
    {

    }

    // entity that is processed by soundsystem will specifically have sound and timer components
    // entity must have either transform or rigidbody, can have both if is 3D
    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, timer& Timer, sound& Sound, transform* Transform, rigidbody* Rigidbody) noexcept
    {
        if (Entity.IsZombie())
            return;
        
        if (Sound.m_SoundID == nullptr)
            return;

        // System Update Code - FOR A SINGLE ENTITY
        auto sound_check = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](const SoundFile& soundfile) { return Sound.m_SoundPlayTag == soundfile.m_ID; });
        //check if id already exists 
        if (sound_check == m_SoundFiles.end())
        {

            //if no, then create new entry and add into record of currently playing sounds
            PlaySoundEvent(std::string_view{Sound.m_SoundID});
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


        //once lifetime over remove sound
        /*Timer.m_Timer -= (Timer.m_Timer > 0.0f) ? DeltaTime() : 0.0f;

        if (Timer.m_Timer <= 0.0f && Timer.m_Timer >= -1.0f)
        {

            Timer.m_Timer = 0.0f;
            
            //find event with tag
            auto current_event = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [Sound](SoundFile& sound_file) { return sound_file.m_ID == Sound.m_SoundPlayTag; });

            //stop event
            if (current_event != m_SoundFiles.end())
            {

                StopSoundEvent(current_event->m_pSound);

                //remove event from event vector
                m_SoundFiles.erase(current_event);
            }

            //delete instance
            //PPB.DeleteEntity(Entity);
        }*/
    }

    //dtor to clean system
    //StopSound("All", true);

    //debug timer for event parameter modification
    float m_TimeTest{ 3.0f };

    //debug function to demonstrate parameter triggered event changes with example fmod bank
    void DebugSoundParameterTest(FMOD::Studio::EventInstance* Sound) {

        //debug timer event toggle
        if (m_TimeTest > 0.0f)
        {

            m_TimeTest -= DeltaTime();
        }

        if (m_TimeTest < 0.0f)
        {

            //value 74 if parameter does not exist
            FMOD_RESULT err = Sound->setParameterByName("Progression", 1.0f);

            ERROR_LOG("SoundSystem Debug Parameter Test Result: " + err);
        }
        else
        {
            ERROR_LOG("SoundSystem Debug Parameter Test: Yet to trigger"); 
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
            else
            {

                //DebugSoundParameterTest(sound.m_pSound);
            }
        }

        //remove all sound files tagged with id 0 since 0 is default tag value which should have been replaced with non-zero from start
        auto end = std::remove_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [](SoundFile& sound) { return sound.m_ID == 0; });

        if (end != std::end(m_SoundFiles))
            m_SoundFiles.erase(end, m_SoundFiles.end());
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
};