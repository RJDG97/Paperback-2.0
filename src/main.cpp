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

    // Initialization
    xcore::Init("Initializing Paperback Engine");
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
            imgui_system,
            debug_system
        >();
    }
    // Entity Creation
    {
        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
            {
                Transform.m_Position.m_X = 5;// std::rand() % (m_Engine.m_Width / 2);
                Transform.m_Position.m_Y = 2.5;// std::rand() % (m_Engine.m_Height / 2);
                Transform.m_Position.m_Z = -10;

                Mesh.m_Model = "Box";
            });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh)
            {
                Transform.m_Position.m_X = -4;// std::rand() % (m_Engine.m_Width / 2);
                Transform.m_Position.m_Y = -2;// std::rand() % (m_Engine.m_Height / 2);
                Transform.m_Position.m_Z = -10;


                Mesh.m_Model = "Box";
            });

        PPB.CreateEntity([&](transform& Transform/*, mesh& Mesh*/, rigidbody& rb)
            {
                Transform.m_Position.m_X = -0.01;// std::rand() % (m_Engine.m_Width / 2);
                Transform.m_Position.m_Y = -4;// std::rand() % (m_Engine.m_Height / 2);
                Transform.m_Position.m_Z = -10;


                //Mesh.m_Model = "Plane";
            });

        PPB.CreateEntity([&](transform& Transform, mesh& Mesh, rigidbody& rb)
            {
                Transform.m_Position.m_X = 2;// std::rand() % (m_Engine.m_Width / 2);
                Transform.m_Position.m_Y = 2;// std::rand() % (m_Engine.m_Height / 2);
                Transform.m_Position.m_Z = -4;
                Transform.fakebox.MinMax[0].Set(paperback::Vector3f(2.f - 2.f, 2.f - 2.f, -4.f - 2.f));
                Transform.fakebox.MinMax[1].Set(paperback::Vector3f(2.f + 2.f, 2.f + 2.f, -4.f + 2.f));
                Transform.fakeSphere.Set(paperback::Vector3f(2.f, 2.f, -4.f), 1.f);
                Transform.m_Offset.setZero();
                Transform.m_Offset.m_Y = 3.0f;


                Mesh.m_Model = "Box";
            });

        PPB.CreateEntity([&](transform& Transform/*, mesh& Mesh*/, rigidbody& rb)
            {
                Transform.m_Position.m_X = 5;// std::rand() % (m_Engine.m_Width / 2);
                Transform.m_Position.m_Y = -1;// std::rand() % (m_Engine.m_Height / 2);
                Transform.m_Position.m_Z = -7;
                Transform.fakebox.MinMax[0].Set(paperback::Vector3f(5.f - 2.f, -1.f - 2.f, -7.f - 2.f));
                Transform.fakebox.MinMax[1].Set(paperback::Vector3f(5.f + 2.f, -1.f + 2.f, -7.f + 2.f));
                Transform.fakeSphere.Set(paperback::Vector3f(5.f, -1.f, -7.f), 1.f);

                //Mesh.m_Model = "Box";
            });


        PPB.SaveScene("test.json");
    }
}
