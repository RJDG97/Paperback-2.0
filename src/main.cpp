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
            rigidforce,
            transform,
            scale,
            rotation,
            Sphere,
            BoundingBox,
            timer,
            sound,
            mesh,
            entityscript
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

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation)
            {
                Transform.m_Position.m_X = 5;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 1;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation)
        {
            Transform.m_Position.m_X = 2;// std::rand() % (m_Engine.m_Width / 2);
            Transform.m_Position.m_Y = 5.8;// std::rand() % (m_Engine.m_Height / 2);
            Transform.m_Position.m_Z = -6;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 1;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
             });

        // Currently running Script to control camera
        PPB.CreateEntity([&](transform& Transform, entityscript& Script)
        {
            Transform.m_Position.m_X = -4;
            Transform.m_Position.m_Y = 2.5;
            Transform.m_Position.m_Z = -10;

            Script.m_ScriptID = "Unit";
        });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation)
            {
                Transform.m_Position.m_X = -4;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 1;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;
        });

        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
        //    {
        //        Transform.m_Position.m_X = -0.01;;
        //        Transform.m_Position.m_Y = -4;
        //        Transform.m_Position.m_Z = -10;


        //        Mesh.m_Model = "Plane";
        //    });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
            {
                Transform.m_Position.m_X = -4;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;


                Mesh.m_Model = "Box";
            });

        PPB.CreateEntity([&](transform& Transform, timer& Timer, sound& Sound)
           {
               Transform.m_Position.m_X = -1;
               Transform.m_Position.m_Y = -4;
               Transform.m_Position.m_Z = -10;

               Timer.m_Timer = -2.0f;

               Sound.m_SoundID = "event:/BGM"; 
           });
        
        // jy
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
        // right most tip
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, BoundingBox& bbox, Sphere& sphere, rigidbody& rb)// rigidforce& rf)
            {
                Transform.m_Position.m_X = -8;
                Transform.m_Position.m_Y = 5.5;
                Transform.m_Position.m_Z = -7;

                bbox.MinMax[0].Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                bbox.MinMax[1].Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                sphere.Set(paperback::Vector3f(
                    Transform.m_Position.m_X,
                    Transform.m_Position.m_Y,
                    Transform.m_Position.m_Z), 1.5f);

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 1;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });
        // right
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, BoundingBox& bbox, Sphere& sphere, rigidbody& rb, rigidforce& rf)
            {
                Transform.m_Position.m_X = -12;
                Transform.m_Position.m_Y = 2.5;
                Transform.m_Position.m_Z = -10;

                bbox.MinMax[0].Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                bbox.MinMax[1].Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                sphere.Set(paperback::Vector3f(
                    Transform.m_Position.m_X,
                    Transform.m_Position.m_Y,
                    Transform.m_Position.m_Z), 1.5f);
                
                rf.m_Mass = 5.0f;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 1;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });
    }
}
