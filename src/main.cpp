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
            debug_system,
            render_system,
            imgui_system
        >();
    }
    // Entity Creation
    {
        PPB.OpenScene("test.json");
    

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
                    Transform.m_Position.m_Z), 1.f);

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 0;
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
                    Transform.m_Position.m_Z), 1.f);
                
                rf.m_Mass = 5.0f;
                rf.m_threshold = 0.5f;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 0;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });
    }
}
