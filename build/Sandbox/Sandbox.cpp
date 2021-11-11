#include "PCH/sandbox_pch.h"

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
    // Register Components
    {
        PPB.RegisterComponents <
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
            entityscript,
            listener,
            parent,
            child,
            offset,
            Collidable,
            Waypointv1,
            WaypointUserv1
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
            //imgui_system,
            animator_system,
            parentchild_system,
            waypoint_system
        >();
    }
    // Entity Creation
    {
        //PPB.OpenScene("../../resources/assetloading/SampleScene.json");

        // Waypoint neeeds -> transform, boundingbox, Collidable, Waypointv1

        // Waypoint users need to just add WaypointUserv1 (player is m_player[0], enemy is m_player[1])

        // Right Box
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, 
            scale& Scale, rotation& Rotation, 
            boundingbox& bbox, sphere& sphere, 
            rigidbody& rb, rigidforce& rf,
            Collidable& collidable, WaypointUserv1& waypointUser)
            {
                Transform.m_Offset.x = 0.f;
                Transform.m_Offset.y = 0.f;
                Transform.m_Offset.z = 0.f;
    
                Transform.m_Position.x = 8;
                Transform.m_Position.y = 5.5;
                Transform.m_Position.z = -7;
    
                bbox.Min.Set(paperback::Vector3f(
                    -1.5f,
                    -1.5f,
                    -1.5f));
    
                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));
    
                sphere.m_fRadius = 1.5f;
    
                rf.m_Mass = 5.0f;
                rf.m_Momentum = paperback::Vector3f(-3.f, 0.f, 0.f);
                rf.m_isStatic = true;
    
                Scale.m_Value.x = 1;
                Scale.m_Value.y = 1;
                Scale.m_Value.z = 1;
    
                Rotation.m_Value.x = 1;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;

                collidable.Set(CollisionLayer::BACKGROUND);

                // player 2
                waypointUser.m_player = 1;
    
                Mesh.m_Model = "Box";
            });
        // Left Box
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, 
            scale& Scale, rotation& Rotation, 
            boundingbox& bbox, sphere& sphere, 
            rigidbody& rb, rigidforce& rf,
            Collidable& collidable, WaypointUserv1& waypointUser)
            {
                
                Transform.m_Offset.x = 0.f;
                Transform.m_Offset.y = 0.f;
                Transform.m_Offset.z = 0.f;
    
                Transform.m_Position.x = -10;
                Transform.m_Position.y = 5.5;
                Transform.m_Position.z = -7.5;
    
                bbox.Min.Set(paperback::Vector3f(
                    -1.5f,
                    -1.5f,
                    -1.5f));
    
                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));
    
                sphere.m_fRadius = 1.5f;
    
                rf.m_Mass = 4.0f;
                rf.m_Momentum = paperback::Vector3f(8.f, 0.f, 0.f);
                
                Scale.m_Value.x = 1;
                Scale.m_Value.y = 1;
                Scale.m_Value.z = 1;
    
                Rotation.m_Value.x = 1;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;

                collidable.Set(1);

                // player 1
                waypointUser.m_player = 0;
    
                Mesh.m_Model = "Box";
            });

        // Waypoint
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh,
            scale& Scale, rotation& Rotation,
            boundingbox& bbox, sphere& sphere,
            Collidable& collidable, Waypointv1& wp)
            {
        
                Transform.m_Offset.x = 0.f;
                Transform.m_Offset.y = 0.f;
                Transform.m_Offset.z = 0.f;
        
                Transform.m_Position.x = -1;
                Transform.m_Position.y = 5.5;
                Transform.m_Position.z = -7.5;
        
                bbox.Min.Set(paperback::Vector3f(
                    -1.5f,
                    -1.5f,
                    -1.5f));
        
                bbox.Max.Set(paperback::Vector3f(
                    1.5f,
                    1.5f,
                    1.5f));
        
                sphere.m_fRadius = 1.5f;
        
                Scale.m_Value.x = 1;
                Scale.m_Value.y = 1;
                Scale.m_Value.z = 1;
        
                Rotation.m_Value.x = 1;
                Rotation.m_Value.y = 0;
                Rotation.m_Value.z = 0;
        
                collidable.Set(1);
        
                wp.m_point = Transform.m_Position; // maybe
                wp.isBase = false; // not impt
                wp.b_enableSpeed = true; // must specify
                wp.m_destination[0] = paperback::Vector3f{ 5.f, 5.5f, -7.f }; // must
                wp.m_destination[1] = paperback::Vector3f{ -7.f, 5.5f, -7.5f };
                wp.Set_d1(3.f); // 3 seconds to target
                wp.Set_d2(3.f);
        
                Mesh.m_Model = "Box";
            });
    }
}
