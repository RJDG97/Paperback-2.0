#include "paperback_pch.h"
#include "Paperback.h"

//-----------------------------------
//       Component & Systems
//-----------------------------------
#include "Components/component_includes.h"
#include "Systems/system_includes.h"

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
    InitializeGame();
    PPB.Initialize();

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
    // Register Components
    {
        PPB.RegisterComponents<
            rigidbody,
            rigidforce,
            transform,
            scale,
            rotation,
            sphere,
            boundingbox,
            timer,
            sound,
            mesh,
            animator,
            entityscript
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system,
            collision_system,
            sound_system,
            scripting_system,
            window_system,
            debug_system,
            render_system,
            imgui_system,
            animator_system
        >();
    }
    // Entity Creation
    {
        PPB.OpenScene("../../resources/assetloading/SampleScene.json");
    
        PPB.OpenScene("test.json");

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, animator& Animator)
            {
                Transform.m_Position.x = -2;
                Transform.m_Position.y = 5;
                Transform.m_Position.z = -10;

                Scale.m_Value.x = 0.03f;
                Scale.m_Value.y = 0.03f;
                Scale.m_Value.z = 0.03f;

                Rotation.m_Value.x = 0;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;

                Mesh.m_Model = "Character";
                Animator.m_CurrentAnimationName = "Armature|Armature|mixamo.com|Layer0";
            });
    }
}
