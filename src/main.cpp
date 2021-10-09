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
            entityscript,
            animator
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
            imgui_system,
            animator_system
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

                bbox.Min.Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                sphere.m_fRadius = 1.0f;

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

                bbox.Min.Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                sphere.m_fRadius = 1.0f;
                
                rf.m_Mass = 5.0f;
                rf.m_threshold = 0.5f;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 0;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, BoundingBox& bbox, Sphere& sphere, rigidbody& rb)// rigidforce& rf)
            {
                Transform.m_Position.m_X = -8;
                Transform.m_Position.m_Y = 5.5;
                Transform.m_Position.m_Z = -7;

                bbox.Min.Set(paperback::Vector3f(
                    - 1.5f,
                    - 1.5f,
                    - 1.5f));

                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));

                sphere.m_fRadius = 1.0f;

                Scale.m_Value.m_X = 1;
                Scale.m_Value.m_Y = 1;
                Scale.m_Value.m_Z = 1;

                Rotation.m_Value.m_X = 0;
                Rotation.m_Value.m_Y = 0;
                Rotation.m_Value.m_Z = 0;

                Mesh.m_Model = "Box";
            });

            PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, animator& Animator)// rigidforce& rf)
                {
                    Transform.m_Position.m_X = -3;
                    Transform.m_Position.m_Y = 5.5;
                    Transform.m_Position.m_Z = -7;

                    Scale.m_Value.m_X = 0.03;
                    Scale.m_Value.m_Y = 0.03;
                    Scale.m_Value.m_Z = 0.03;

                    Rotation.m_Value.m_X = 0;
                    Rotation.m_Value.m_Y = 0;
                    Rotation.m_Value.m_Z = 0;

                    Mesh.m_Model = "Character";
                    Animator.m_CurrentAnimationName = "Armature|Armature|mixamo.com|Layer0";
                });
    }
}
