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


int main( int argc, char* argv[] )
{
#if defined( PAPERBACK_DEBUG ) | defined( PAPERBACK_DEBUG )
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    ShowWindow( GetConsoleWindow(), SW_SHOW );
#else
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    // Initialization
    xcore::Init( "Initializing Paperback Engine");
    InitializeGame();

    PPB.Update();

    // Termination
    PPB.Terminate();
    xcore::Kill();
}













void InitializeGame()
{
    // Register Components
    {
       PPB.RegisterComponents<
            bullet,
            rigidbody,
            transform,
            timer,
            sound,
            sample_tag_component,
            mesh
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system,
            ship_logic_system,
            bullet_logic_system,
            sound_system,
            scripting_system,
            window_system,
            render_system,
            imgui_system

        >();
    }
    // Entity Creation
    {
         for (int i = 0; i < 5; ++i)
         {
             PPB.CreateEntity( [&]( transform& Transform, rigidbody& RigidBody, timer& Timer )
                               {
                                   Transform.m_Position.m_X = std::rand() % m_Engine.m_Width;
                                   Transform.m_Position.m_Y = std::rand() % m_Engine.m_Height;

                                   RigidBody.m_Velocity.m_X = ( std::rand() % 40 );
                                   RigidBody.m_Velocity.m_Y = ( std::rand() % 40 );

                                   Timer.m_Timer = (std::rand() / (float)RAND_MAX) * 8;
                               });
         }


         PPB.SaveScene("test.json");
    }
}
