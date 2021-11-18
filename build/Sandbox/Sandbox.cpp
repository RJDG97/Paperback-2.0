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
        ,    waypoint_tag
        ,    Frustum
        ,    LineSegment
        ,    mass
        ,    Plane
        ,    Ray
        ,    Triangle
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system
        ,   collision_system
        //,   resolution_system
        ,   sound_system
        ,   scripting_system
        ,   window_system
        ,   debug_system
        ,   render_system
        ,   imgui_system
        ,   animator_system
        ,   parentchild_system
        ,   player_spawner_system
        ,   enemy_spawner_system
        ,   timer_system
        ,   waypoint_system
        >();

        PPB.RegisterSystems <
            onevent_UnitTrigger_system
        ,   onevent_UnitTriggerStay_system
        ,   onevent_UnitTriggerExit_system
        ,   onevent_ParentDeleted_system
        ,   onevent_ChildDeleted_system
        >();
    }
    // Entity Creation
    {
		PPB.OpenScene();

        //// Right Box
        //PPB.CreateEntity([&](transform& Transform, mesh& Mesh,
        //    scale& Scale, rotation& Rotation,
        //    boundingbox& bbox, //Ray& ray, 
        //    rigidbody& rb, rigidforce& rf)
        //    {
        //        Transform.m_Offset.x = 0.f;
        //        Transform.m_Offset.y = 0.f;
        //        Transform.m_Offset.z = 0.f;
        //
        //        Transform.m_Position.x = 8;
        //        Transform.m_Position.y = 5.5;
        //        Transform.m_Position.z = -7;
        //
        //        bbox.Min.Set(paperback::Vector3f(
        //            -1.5f,
        //            -1.5f,
        //            -1.5f));
        //
        //        bbox.Max.Set(paperback::Vector3f(
        //            1.5f,
        //            1.5f,
        //            1.5f));
        //
        //        //line.m_Start = paperback::Vector3f(0.f, 0.f, 0.f);
        //        //line.m_End = paperback::Vector3f(10.f, 0.f, 0.f);
        //
        //        //tri.m_pointA = paperback::Vector3f(-2.5f, -2.5f, -2.5f);
        //        //tri.m_pointB = paperback::Vector3f(2.5f, -2.5f, -2.5f);
        //        //tri.m_pointC = paperback::Vector3f(0.f, 2.5f, -2.5f);
        //
        //        //frus.m_pointA = paperback::Vector3f(-2.5f, -2.5f, -2.5f);
        //        //frus.m_pointB = paperback::Vector3f(2.5f, -2.5f, -2.5f);
        //        //frus.m_pointC = paperback::Vector3f(2.5f, 2.5f, -2.5f);
        //        //frus.m_pointD = paperback::Vector3f(-2.5f, 2.5f, -2.5f);
        //        //frus.m_pointE = paperback::Vector3f(-2.5f, -2.5f, 2.5f);
        //        //frus.m_pointF = paperback::Vector3f(2.5f, -2.5f, 2.5f);
        //        //frus.m_pointG = paperback::Vector3f(2.5f, 2.5f, 2.5f);
        //        //frus.m_pointH = paperback::Vector3f(-2.5f, 2.5f, 2.5f);
        //
        //        //ray.m_Start = paperback::Vector3f(0.f, 0.f, 0.f);
        //        //ray.m_Direction = paperback::Vector3f(5.f, 0.f, 0.f);
        //
        //        //plane.Set(paperback::Vector3f(-2.5f, -2.5f, -2.5f),
        //        //    paperback::Vector3f(2.5f, -2.5f, -2.5f), 
        //        //    paperback::Vector3f(0.f, 2.5f, -2.5f));
        //
        //        rf.m_Mass = 5.0f;
        //        rf.m_Momentum = paperback::Vector3f(-5.f, 0.f, 0.f);
        //        //rf.m_isStatic = true;
        //
        //        Scale.m_Value.x = 1;
        //        Scale.m_Value.y = 1;
        //        Scale.m_Value.z = 1;
        //
        //        Rotation.m_Value.x = 1;
        //        Rotation.m_Value.y = 0;
        //        Rotation.m_Value.z = 0;
        //
        //        Mesh.m_Model = "Box";
        //    });
    }
}
