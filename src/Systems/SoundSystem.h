#pragma once
#include "Components/Transform.h"
#include "Components//Rigidbody.h"
#include "Components/Sound.h"
#include "Components/Timer.h"

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
    void AddBank(const char* filename) 
    {

        FMOD::Studio::Bank* result;
        FMOD_RESULT res = m_pStudioSystem->loadBankFile(filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &result);

        if (!result) 
        {

            std::cout << "Bad read: " << filename << std::endl;
        }

        std::cout << "bank load status: " << res << "\n\n";
    }

    void RemoveAllBanks() 
    {

        m_pStudioSystem->unloadAll();
    }

    //play event 
    // helper function
    // loads and plays an event from the current loaded bank
    void PlaySoundEvent(const char* path) 
    {

        FMOD::Studio::EventDescription* event = nullptr;
        
        if (m_pStudioSystem->getEvent(path, &event) == FMOD_OK) 
        {

            m_SoundFiles.push_back({});
            m_SoundFiles.back().m_ID = ++m_SoundCounter;

            //FMOD::Studio::EventInstance* instance = nullptr;
            //event->loadSampleData();
            event->createInstance(&m_SoundFiles.back().m_pSound);
            FMOD_RESULT result = m_SoundFiles.back().m_pSound->start(); 
            std::cout << result << std::endl;
            m_SoundFiles.back().m_pSound->setVolume(1.0f);

            //in case of extra load case to debug log
            FMOD_STUDIO_PLAYBACK_STATE be;
            m_SoundFiles.back().m_pSound->getPlaybackState(&be);

            if (be != 0)
                std::cout << be << std::endl;
        }
        else {

            std::cout << "event not found" << std::endl;
        }
    }

    //stop sound
    // helper function
    // stops sound from currently playing
    void StopSoundEvent(FMOD::Studio::EventInstance* instance) 
    {

        instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        instance->release();
    }

    //pause sound
    // helper function
    // pauses/unpauses sound channels
    void PauseSoundEvent(FMOD::Studio::EventInstance* instance)
    {
        bool paused_status;
        instance->getPaused(&paused_status);

        if (paused_status)
        {
            instance->setPaused(true);
        }
        else
        {
            instance->setPaused(false);
        }
    }

    //convert transform & vel to fmod 3d attribute
    //helper function
    //to modify when 3d switch
    void ConvertSystemToFMOD3D(FMOD_3D_ATTRIBUTES& attribute, const transform& transform, const rigidbody& rigidbody)
    {
        //2d now, to change to 3d when shifted to 3d
        xcore::vector2 normal_vec = rigidbody.m_Velocity;
        normal_vec.Normalize();

        attribute.forward.x = normal_vec.m_X;
        attribute.forward.y = normal_vec.m_Y;

        //attribute.up.z = 0.0f;

        attribute.position.x = transform.m_Position.m_X;
        attribute.position.y = transform.m_Position.m_Y;

        attribute.velocity.x = rigidbody.m_Velocity.m_X;
        attribute.velocity.y = rigidbody.m_Velocity.m_Y;
    }

    //set player 3d attributes
    // helper function
    // sets the player's FMOD 3d attributes for 3D sound
    void UpdatePlayer3DAttributes(const transform& transform, const rigidbody& rigidbody) 
    {

        FMOD_3D_ATTRIBUTES attribute{};
        ConvertSystemToFMOD3D(attribute, transform, rigidbody);

        m_pStudioSystem->setListenerAttributes(0, &attribute);
    }

    //set 3D attributes
    // helper function
    // sets a sound instance's 3d attributes
    void UpdateEvent3DAttributes(FMOD::Studio::EventInstance* instance, const transform& transform, const rigidbody& rigidbody)
    {

        FMOD_3D_ATTRIBUTES attribute{};
        ConvertSystemToFMOD3D(attribute, transform, rigidbody);

        instance->set3DAttributes(&attribute);
    }

    //pause/unpause all sounds
    // helper function
    // pauses/unpauses all currently playing sounds
    void PauseCurrentSounds(const bool pause_status)
    {

        for (SoundFile& sound : m_SoundFiles) {

            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);

            //if sound is not stopped, set pause status
            if (be != 2) {

                sound.m_pSound->setPaused(pause_status);
            }
        }
    }

    //Change Event Parameter
    // helper function
    // changes the value of a currently playing event
    // ideally has a separate logic calling this when required
    void ChangeEventParameters(FMOD::Studio::EventInstance* instance, const char* param_name, const float value)
    {

        instance->setParameterByName(param_name, value);
    }


    //Debug test bank loader
    // helper function
    // used for debug loading of a test bank
    void LoadDebugBank()
    {

        AddBank("TestBank/Master.bank");
        AddBank("TestBank/Master.strings.bank");
        AddBank("TestBank/Dialogue_EN.bank");
        AddBank("TestBank/Music.bank");
        AddBank("TestBank/SFX.bank");
        AddBank("TestBank/Vehicles.bank");
        AddBank("TestBank/VO.bank");

        //PlaySoundEvent("event:/Abang", false);
        PlaySoundEvent("event:/Music/Level 01");
    }

    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "sound_system"
    };

    PPB_FORCEINLINE
    void OnSystemCreated( void ) noexcept
    {
        // set up system aka fmod required stuff
        //FMOD::System_Create(&m_pFMODSystem);
        //m_pFMODSystem->init(32, FMOD_INIT_NORMAL, m_pFMODSystem);
        
        //create fmod studio system instance
        FMOD::Studio::System::create(&m_pStudioSystem);
        m_pStudioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);

        // store pointer to underlying fmod system
        m_pStudioSystem->getCoreSystem(&m_pFMODSystem);

        //add master bank file
        //AddBank("Master.bank");
        //AddBank("Master.strings.bank");
        //LoadDebugBank();

        m_SoundCounter = {};

        m_pStudioSystem->setNumListeners(1);
    }

    // entity that is processed by soundsystem will specifically have sound and timer components
    PPB_FORCEINLINE
    void operator()(paperback::component::entity& Entity, timer& timer, sound& sound, transform& transform, rigidbody& rigidbody) noexcept
    {
        //to change from & to * for transform & rigidbody, worst case make a system that is derived from this one

        // System Update Code - FOR A SINGLE ENTITY

        //check if id already exists 
        if (std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [sound](const SoundFile& soundfile) { return sound.m_SoundPlayTag == soundfile.m_ID; }) == m_SoundFiles.end()) 
        {

            //if no, then create new entry and add into record of currently playing sounds
            PlaySoundEvent(sound.m_SoundID.c_str());
            sound.m_SoundPlayTag = m_SoundCounter;
        }


        //process sound based on position of listener
        if (sound.m_Is3DSound)//  && transform && rigidbody)
        {

            //file should exist, no need to error check
            auto soundfile = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [sound](const SoundFile& soundfile) { return sound.m_SoundPlayTag == soundfile.m_ID; });
            
            UpdateEvent3DAttributes(soundfile->m_pSound, transform, rigidbody);
        }


        //once lifetime over remove sound
        timer.m_Timer -= (timer.m_Timer > 0.0f) ? DeltaTime() : 0.0f;

        if (timer.m_Timer <= 0.0f && timer.m_Timer >= -1.0f)
        {

            timer.m_Timer = 0.0f;
            
            //find event with tag
            auto current_event = std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [sound](SoundFile& sound_file) { return sound_file.m_ID == sound.m_SoundPlayTag; });

            //stop event
            if (current_event != m_SoundFiles.end())
            {

                StopSoundEvent(current_event->m_pSound);
            }

            //remove event from event vector
            m_SoundFiles.erase(current_event);

            //delete instance
        }
    }

    //dtor to clean system
    //StopSound("All", true);

    //debug timer for event parameter modification
    float m_TimeTest{ 3.0f };

    //debug function to demonstrate parameter triggered event changes with example fmod bank
    void DebugSoundParameterTest(FMOD::Studio::EventInstance* sound) {

        //debug timer event toggle
        if (m_TimeTest > 0.0f)
        {

            m_TimeTest -= DeltaTime();
        }

        if (m_TimeTest < 0.0f)
        {

            //value 74 if parameter does not exist
            FMOD_RESULT err = sound->setParameterByName("Progression", 1.0f);

            std::cout << "\tParameter get result: " << err << std::endl;

        }
        else
        {
            std::cout << "\tYet to trigger\n";
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
        std::remove_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [](SoundFile& sound) { return sound.m_ID == 0; });
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