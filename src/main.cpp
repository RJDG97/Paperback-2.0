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
            sphere,
            boundingbox,
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
            collision_system,
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
    
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, boundingbox& Bbox, sphere& Sphere, rigidbody& RB)// rigidforce& rf)
            {
                Transform.m_Position.x = -8;
                Transform.m_Position.y = 5.5;
                Transform.m_Position.z = -7;

                Bbox.Min.Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                Bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                Sphere.m_fRadius = 1.0f;

                Scale.m_Value.x = 1;
                Scale.m_Value.y = 1;
                Scale.m_Value.z = 1;

                Rotation.m_Value.x = 0;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;

                Mesh.m_Model = "Box";
            });
        // right
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, boundingbox& Bbox, sphere& Sphere, rigidbody& RB, rigidforce& RF)
            {
                Transform.m_Position.x = -12;
                Transform.m_Position.y = 2.5;
                Transform.m_Position.z = -10;

                Bbox.Min.Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                Bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                Sphere.m_fRadius = 1.0f;
                
                RF.m_Mass = 5.0f;
                RF.m_threshold = 0.5f;

                Scale.m_Value.x = 1;
                Scale.m_Value.y = 1;
                Scale.m_Value.z = 1;

                Rotation.m_Value.x = 0;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;

                Mesh.m_Model = "Box";
            });
    }
}
