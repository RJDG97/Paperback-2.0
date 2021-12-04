#include "PCH/sandbox_pch.h"

//-----------------------------------
//       Component & Systems
//-----------------------------------
#include "Components/component_includes.h"
#include "Systems/system_includes.h"
#include "Scripts/scripts_includes.h"

//-----------------------------------
//      Forward Declarations
//-----------------------------------
using namespace paperback;
void InitializeGame();

int main(int argc, char* argv[])
{
#if defined( PAPERBACK_DEBUG ) | defined( PAPERBACK_DEBUG )
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    /*
        Initialization
    */
    xcore::Init("Initializing Paperback Engine");
    PPB.Initialize();
    InitializeGame();

    /*
        Main Loop
    */
    PPB.Update();

    /*
        Terminate
    */
    PPB.Terminate();
    xcore::Kill();
}













void InitializeGame()
{
    // Register Components - Add to the end of the list
    {
        PPB.RegisterComponents<
             mesh
        ,    scale
        ,    timer
        ,    sound
        ,    child
        ,    parent
        ,    offset
        ,    prefab
        ,    sphere
        ,    listener
        ,    rotation
        ,    animator
        ,    rigidbody
        ,    transform
        ,    rigidforce
        ,    entityscript
        ,    boundingbox
        ,    reference_prefab
        ,    collidable
        ,    waypoint
        ,    waypointv1
        ,    unitstate
        ,    selected
        ,    health
        ,    damage
        ,    currency
        ,    cost
        ,    socketed
        ,    name
        ,    counter
        ,    shield
        ,    friendly         // Tag
        ,    enemy            // Tag
        ,    friendly_spawner // Tag
        ,    enemy_spawner    // Tag
        ,    player           // Tag
        ,    base             // Tag
        ,    healthbar        // Tag
        ,    unit             // Tag
        ,    waypoint_tag
        ,    text
        ,    button
        ,    card
        ,    deck
        ,    path
        ,    path_follower
        ,    spawner
        ,    mass
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system
        ,   collision_system
        ,   sound_system
        ,   scripting_system
        ,   window_system
        ,   debug_system
        ,   path_system
        ,   render_system
        ,   animator_system
        ,   parentchild_system
        ,   player_spawner_system
        ,   enemy_spawner_system
        ,   timer_system
        ,   waypoint_system
        ,   health_system
        ,   ui_system
        ,   deck_system
        ,   imgui_system
        ,   camera_system
        >();

        PPB.RegisterSystems <
            onevent_UnitTrigger_system
        ,   onevent_UnitTriggerStay_system
        ,   onevent_UnitTriggerExit_system
        ,   onevent_NoHealth_system
        ,   onevent_UpdateHealth_system
        ,   onevent_ParentDeleted_system
        ,   onevent_ChildDeleted_system
        ,   onevent_ReferencePrefabDeleted_system
        >();

        PPB.RegisterScripts<
            card_script,
            levelselect_button_mainmenu_script,
            credits_button_mainmenu_script,
            how2play_button_mainmenu_script,
            mainmenu_button_script,
            quit_button_script,
            settings_button_mainmenu_script,
            quitwindow_button_mainmenu_script,
            quitcancel_button_mainmenu_script,
            pausecancel_button_game_script,
            pauseenable_button_game_script,
            quitcancel_button_game_script,
            quitwindow_button_game_script,
            returnmainmenu_cancel_button_game_script,
            returnmainmenu_window_button_game_script,
            startgame_button_script
        >();
    }
    // Entity Creation
    {
		PPB.OpenScene();
    }
}
