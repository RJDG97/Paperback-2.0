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
    ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

    /*
        Initialization
    */
    xcore::Init( "Initializing Paperback Engine");
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
            transform,
            timer,
            sound,
            mesh
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system,
            bullet_logic_system,
            sound_system,
            scripting_system,
            window_system,
            render_system,
            imgui_system,
            debug_system
        >();
    }
    // Entity Creation
    {
        {
            //for ( int i = 0; i < 3; ++i )
            //{
            //    // Add Bullet Component Test
            //    PPB.CreateEntity([&](transform& Transform, rigidbody& RB)
            //        {
            //            UNREFERENCED_PARAMETER(Transform);
            //            UNREFERENCED_PARAMETER(RB);
            //        });
            //}

            //// Remove Bullet Component Test
            // for ( int i = 0; i < 3; ++i )
            //{
            //    PPB.CreateEntity([&](transform& Transform, rigidbody& RB, bullet& Bullet)
            //        {
            //            UNREFERENCED_PARAMETER(Transform);
            //            UNREFERENCED_PARAMETER(RB);
            //            UNREFERENCED_PARAMETER(Bullet);
            //        });
            //}
        }

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
            {
                Transform.m_Position.m_X = 5;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;

        //        Mesh.m_Model = "Box";
        //    });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
        {
            Transform.m_Position.m_X = 6;// std::rand() % (m_Engine.m_Width / 2);
            Transform.m_Position.m_Y = 8.5;// std::rand() % (m_Engine.m_Height / 2);
            Transform.m_Position.m_Z = 0;

            Mesh.m_Model = "Box";
        });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
            {
                Transform.m_Position.m_X = -4;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;


                Mesh.m_Model = "Box";
            });

        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
        //    {
        //        Transform.m_Position.m_X = -0.01;;
        //        Transform.m_Position.m_Y = -4;
        //        Transform.m_Position.m_Z = -10;


        //        Mesh.m_Model = "Plane";
        //    });

        PPB.CreateEntity([&](transform& Transform, timer& Timer, sound& Sound)
            {
                Transform.m_Position.m_X = -1;
                Transform.m_Position.m_Y = -4;
                Transform.m_Position.m_Z = -10;

                Timer.m_Timer = -2.0f;

                Sound.m_SoundID = "event:/BGM";
            });
        //
        // jjyy
        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
        //    {
        //        Transform.m_Position.m_X = 2;// std::rand() % (m_Engine.m_Width / 2);
        //        Transform.m_Position.m_Y = 2;// std::rand() % (m_Engine.m_Height / 2);
        //        Transform.m_Position.m_Z = -4;
        //        Transform.fakebox.MinMax[0].Set(paperback::Vector3f(
        //            Transform.m_Position.m_X - 2.f,
        //            Transform.m_Position.m_Y - 2.f,
        //            Transform.m_Position.m_Z - 2.f));
        //        Transform.fakebox.MinMax[1].Set(paperback::Vector3f(
        //            Transform.m_Position.m_X + 2.f,
        //            Transform.m_Position.m_Y + 2.f,
        //            Transform.m_Position.m_Z + 2.f));
        //        Transform.fakeSphere.Set(paperback::Vector3f(
        //            Transform.m_Position.m_X,
        //            Transform.m_Position.m_Y,
        //            Transform.m_Position.m_Z), 1.f);
        //
        //        Mesh.m_Model = "Box";
        //    });
        //
        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
        //    {
        //        Transform.m_Position.m_X = 5;// std::rand() % (m_Engine.m_Width / 2);
        //        Transform.m_Position.m_Y = -1;// std::rand() % (m_Engine.m_Height / 2);
        //        Transform.m_Position.m_Z = -7;
        //        Transform.fakebox.MinMax[0].Set(paperback::Vector3f(Transform.m_Position.m_X - 2.f,
        //            Transform.m_Position.m_Y - 2.f,
        //            Transform.m_Position.m_Z - 2.f));
        //        Transform.fakebox.MinMax[1].Set(paperback::Vector3f(Transform.m_Position.m_X + 2.f,
        //            Transform.m_Position.m_Y + 2.f,
        //            Transform.m_Position.m_Z + 2.f));
        //        Transform.fakeSphere.Set(paperback::Vector3f(Transform.m_Position.m_X,
        //            Transform.m_Position.m_Y,
        //            Transform.m_Position.m_Z),
        //            1.f);
        //
        //        Mesh.m_Model = "Box";
        //    });
    }
}
