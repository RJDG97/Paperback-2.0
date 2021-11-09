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
        ,    selected
        ,    health
        ,    damage
        ,    currency
        ,    cost
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
            animator_system,
            parentchild_system
        >();

        PPB.RegisterSystems <
            onevent_movement_system
        >();
    }
    // Entity Creation
    {
        //PPB.LoadEntityInfo("../../resources/assetloading/EntityInfoTest.json");
        //PPB.OpenScene("../../resources/assetloading/ParentingTest.json");

        //// Right Box
        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, boundingbox& bbox, sphere& sphere, rigidbody& rb, rigidforce& rf)
        //    {
        //        Transform.m_Offset.x = 0.f;
        //        Transform.m_Offset.y = 0.f;
        //        Transform.m_Offset.z = 0.f;
    
        //        Transform.m_Position.x = 5;
        //        Transform.m_Position.y = 5.5;
        //        Transform.m_Position.z = -7;
    
        //        bbox.Min.Set(paperback::Vector3f(
        //            -1.5f,
        //            -1.5f,
        //            -1.5f));
    
        //        bbox.Max.Set(paperback::Vector3f(
        //            1.5f,
        //            1.5f,
        //            1.5f));
    
        //        sphere.m_fRadius = 1.5f;
    
        //        rf.m_Mass = 5.0f;
        //        rf.m_Momentum = paperback::Vector3f(-3.f, 0.f, 0.f);
        //        rf.m_isStatic = true;
    
        //        Scale.m_Value.x = 1;
        //        Scale.m_Value.y = 1;
        //        Scale.m_Value.z = 1;
    
        //        Rotation.m_Value.x = 1;
        //        Rotation.m_Value.y = 0;
        //        Rotation.m_Value.z = 0;
    
        //        Mesh.m_Model = "Box";
        //    });
        //// Left Box
        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh, scale& Scale, rotation& Rotation, boundingbox& bbox, sphere& sphere, rigidbody& rb, rigidforce& rf)
        //    {
        //        
        //        Transform.m_Offset.x = 0.f;
        //        Transform.m_Offset.y = 0.f;
        //        Transform.m_Offset.z = 0.f;
    
        //        Transform.m_Position.x = -7;
        //        Transform.m_Position.y = 5.5;
        //        Transform.m_Position.z = -7.5;
    
        //        bbox.Min.Set(paperback::Vector3f(
        //            -1.5f,
        //            -1.5f,
        //            -1.5f));
    
        //        bbox.Max.Set(paperback::Vector3f(
        //            1.5f,
        //            1.5f,
        //            1.5f));
    
        //        sphere.m_fRadius = 1.5f;
    
        //        rf.m_Mass = 4.0f;
        //        rf.m_Momentum = paperback::Vector3f(8.f, 0.f, 0.f);
        //        
        //        Scale.m_Value.x = 1;
        //        Scale.m_Value.y = 1;
        //        Scale.m_Value.z = 1;
    
        //        Rotation.m_Value.x = 1;
        //        Rotation.m_Value.y = 0;
        //        Rotation.m_Value.z = 0;
    
        //        Mesh.m_Model = "Box";
        //    });
    }
}
