#pragma once
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
        m_pStudioSystem->loadBankFile(filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &result);

        if (!result) 
        {

            std::cout << "Bad read\n" << std::endl;
        }
    }

    void RemoveAllBanks() 
    {

        m_pStudioSystem->unloadAll();
    }

    //play event 
    // helper function
    // loads and plays an event from the current loaded bank
    void PlaySoundEvent(const char* path, bool loop) 
    {

        FMOD::Studio::EventDescription* event = nullptr;
        
        if (m_pStudioSystem->getEvent(path, &event) == FMOD_OK) 
        {
            std::cout << "event found\n";
            m_SoundFiles.push_back({});
            m_SoundFiles.back().m_ID = ++m_SoundCounter;

            //FMOD::Studio::EventInstance* instance = nullptr;
            //event->loadSampleData();
            event->createInstance(&m_SoundFiles.back().m_pSound);
            FMOD_RESULT result = m_SoundFiles.back().m_pSound->start(); 
            std::cout << result << std::endl;
            m_SoundFiles.back().m_pSound->setVolume(1.0f);

            FMOD_STUDIO_PLAYBACK_STATE be;
            m_SoundFiles.back().m_pSound->getPlaybackState(&be);

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
        AddBank("Master.bank");
        AddBank("Master.strings.bank");

        PlaySoundEvent("event:/Abang", false);

        m_SoundCounter = {};
    }


    PPB_FORCEINLINE
    void Update( void ) noexcept
    {
        // call fmod default stuff IF there's something that needs to be "globally" called 
        m_pStudioSystem->update();

        for (SoundFile& sound : m_SoundFiles) {

            FMOD_STUDIO_PLAYBACK_STATE be;
            sound.m_pSound->getPlaybackState(&be);

            std::cout << be << std::endl;
        }
    }

    // entity that is processed by soundsystem will specifically have sound and timer components
    PPB_FORCEINLINE
        void operator()(paperback::component::entity& Entity, timer& timer, sound& sound) noexcept
    {
        // System Update Code - FOR A SINGLE ENTITY

        //check if id already exists 
        if (std::find_if(std::begin(m_SoundFiles), std::end(m_SoundFiles), [sound](const SoundFile& soundfile) { return sound.m_SoundPlayTag == soundfile.m_ID; }) == m_SoundFiles.end()) 
        {

            //if no, then create new entry and add into record of currently playing sounds
            PlaySoundEvent(sound.m_SoundID.c_str(), false);
            sound.m_SoundPlayTag = m_SoundCounter;
        }


        //process sound based on position of listener

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

    // Terminate system
    PPB_FORCEINLINE
    void OnFrameEnd(void) noexcept 
    {
        m_pStudioSystem->unloadAll();
        m_pStudioSystem->release();
    }


};