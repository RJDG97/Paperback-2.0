#include "PCH/sandbox_pch.h"

//-----------------------------------
//       Component & Systems
//-----------------------------------
#include "Components/component_includes.h"
#include "Systems/system_includes.h"
#include "Scripts/scripts_includes.h"
#include "InputBindings/binding_includes.h"

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
    ShowWindow(GetConsoleWindow(), SW_SHOW);
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
        ,    capture_point
        ,    Frustum
        ,    LineSegment
        ,    Plane
        ,    Ray
        ,    Triangle
        ,    spawner_details
        ,    icon             // Tag
        ,    ui
        ,    camera
        ,    player_controller
        ,    slope
        ,    bounding_volume // Tag
        ,    elevator
        ,    player_interaction
        ,    pushable        // Tag
        ,    crosshair       // Tag
        ,    light
        ,    bulksound
        ,    cinematic
        ,    ability_collectible
        ,    particle_emitter
        ,    particle
        ,    dialogue_text
        ,    dialogue_collider
        >();

        // Register Components - Add to the end of the list
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            debug_system
        ,   path_system
        ,   physics_system
        ,   parentchild_system
        ,   scripting_system
        ,   collision_system
        ,   sound_system
        ,   window_system
        ,   ui_system
        ,   particle_system
        ,   render_system
        ,   animator_system
        ,   timer_system
        ,   imgui_system
        ,   camera_system
        ,   icon_system
        ,   player_camera_system
        ,   dialogue_system
        >();

        PPB.RegisterSystems <
            onevent_UnitTrigger_system
        ,   onevent_UnitTriggerStay_system
        ,   onevent_UnitTriggerExit_system
        ,   onevent_ParentDeleted_system
        ,   onevent_ChildDeleted_system
        ,   onevent_ReferencePrefabDeleted_system
        ,   onevent_PointCaptured_system
        ,   onevent_CapturePointDamaged_system
        ,   onevent_ResetAnimation
        ,   onevent_FallingAnimation
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
            startgame_button_script,
            how2play_cancel_button_mainmenu_script,
            page1_how2play_next_button_mainmenu_script,
            page1_how2play_prev_button_mainmenu_script,
            page2_how2play_next_button_mainmenu_script,
            page2_how2play_prev_button_mainmenu_script,
            page3_how2play_next_button_mainmenu_script,
            page3_how2play_prev_button_mainmenu_script,
            page4_how2play_next_button_mainmenu_script,
            page4_how2play_prev_button_mainmenu_script,
            page5_how2play_next_button_mainmenu_script,
            page5_how2play_prev_button_mainmenu_script,
            page6_how2play_next_button_mainmenu_script,
            page6_how2play_prev_button_mainmenu_script,
            mainmenu_mode_script,
            game_mode_script,
            nextlevel_level1_button_script,
            startlevel1_button_script,
            startlevel2_button_script,
            startlevel3_button_script,
            startcutscene_mid_button_script,
            startcutscene_last_button_script,
            nextlevel_level2_button_script,
            nextlevel_level3_button_script
        >();


        /************************************************************************************/
        //                 All Control Bindings Should Be Serialized w/ Guids
        /************************************************************************************/
        {
            // Keyboard Registration
            auto Keyboard_Movement_Forward   = PPB.RegisterBinding<paperback::input::binding::Keyboard_EntityMovement_Forward>();
            auto Keyboard_Movement_Backwards = PPB.RegisterBinding<paperback::input::binding::Keyboard_EntityMovement_Backwards>();
            auto Keyboard_Movement_Left      = PPB.RegisterBinding<paperback::input::binding::Keyboard_EntityMovement_Left>();
            auto Keyboard_Movement_Right     = PPB.RegisterBinding<paperback::input::binding::Keyboard_EntityMovement_Right>();

            // Mouse Registration
            auto Mouse_Rotate                = PPB.RegisterBinding<paperback::input::binding::Mouse_Camera_Rotate>();

            // Gamepad Registration
            auto Gamepad_Movement            = PPB.RegisterBinding<paperback::input::binding::Gamepad_EntityMovement>();
            auto Gamepad_Rotate              = PPB.RegisterBinding<paperback::input::binding::Gamepad_Camera_Rotate>();

            // Generic Gameplay Registration
            auto Jump_Action                 = PPB.RegisterBinding<paperback::input::binding::Jump_Action>();
            auto ToggleLift_Action           = PPB.RegisterBinding<paperback::input::binding::Lift_Action>();
            auto Release_Action              = PPB.RegisterBinding<paperback::input::binding::Release_Action>();
            auto PushPull_Action             = PPB.RegisterBinding<paperback::input::binding::PushPull_Action>();
            auto Toggle_FPSAction            = PPB.RegisterBinding<paperback::input::binding::Toggle_FPS_Action>();
            auto Toggle_Fullscreen           = PPB.RegisterBinding<paperback::input::binding::Toggle_Fullscreen>();
            auto Toggle_Players              = PPB.RegisterBinding<paperback::input::binding::Toggle_Players>();



            // Keyboard Bindings
            PPB.AssignBindingToAction( Keyboard_Movement_Forward,   GLFW_KEY_W,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( Keyboard_Movement_Backwards, GLFW_KEY_S,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( Keyboard_Movement_Left,      GLFW_KEY_A,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( Keyboard_Movement_Right,     GLFW_KEY_D,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( PushPull_Action,             GLFW_KEY_W,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( PushPull_Action,             GLFW_KEY_S,     input::device::type::id::KEYBOARD );
            PPB.AssignBindingToAction( Toggle_FPSAction,            GLFW_KEY_E,     input::device::type::id::KEYBOARD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( Jump_Action,                 GLFW_KEY_SPACE, input::device::type::id::KEYBOARD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( ToggleLift_Action,           GLFW_KEY_SPACE, input::device::type::id::KEYBOARD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( Toggle_Players,              GLFW_KEY_Q,     input::device::type::id::KEYBOARD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( Toggle_Fullscreen,           GLFW_KEY_F,     input::device::type::id::KEYBOARD, paperback::input::action::BroadcastStatus::PRESSED
                                                                                                                     , paperback::input::action::KeyPairing::PPB_CTRL_KEY );


            // Mouse Bindings
            PPB.AssignBindingToAction( Mouse_Rotate, GLFW_MOUSE_BUTTON_3, input::device::type::id::MOUSE );


            // Gamepad Bindings
            PPB.AssignBindingToAction( Gamepad_Movement,  GLFW_GAMEPAD_BUTTON_LEFT_THUMB,  input::device::type::id::GAMEPAD );
            PPB.AssignBindingToAction( Gamepad_Rotate,    GLFW_GAMEPAD_BUTTON_RIGHT_THUMB, input::device::type::id::GAMEPAD );
            PPB.AssignBindingToAction( Jump_Action,       GLFW_GAMEPAD_BUTTON_B,           input::device::type::id::GAMEPAD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( ToggleLift_Action, GLFW_GAMEPAD_BUTTON_B,           input::device::type::id::GAMEPAD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( Toggle_FPSAction,  GLFW_GAMEPAD_BUTTON_A,           input::device::type::id::GAMEPAD, paperback::input::action::BroadcastStatus::PRESSED );
            PPB.AssignBindingToAction( Toggle_Players,    GLFW_GAMEPAD_BUTTON_Y,           input::device::type::id::GAMEPAD, paperback::input::action::BroadcastStatus::PRESSED );
        }
    }
    // Set Window maximized initially
    auto& Window = PPB.GetSystem< window_system >();
    glfwMaximizeWindow(Window.m_pWindow);
    // Entity Creation
    {
		PPB.OpenScene();
    }
}
