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
void GlutTimer(int value);
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

    // Game Loop - To be replaced with purely PPB.Update()
    // when rendering system is up
    glutInitWindowSize( m_Engine.m_Width, m_Engine.m_Height );
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE );
    glutCreateWindow( "Paperback Engine" );
    glutDisplayFunc([]
    {
        PPB.Update();
    });
    glutReshapeFunc([](int w, int h)
    {
        m_Engine.m_Width = w;
        m_Engine.m_Height = h;
        glViewport( 0, 0, w, h );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, w, 0, h, -1, 1 );
        glScalef( 1, -1, 1 );
        glTranslatef( 0, -h, 0 );
    });
    glutTimerFunc( 0, GlutTimer, 0 );
    glutMainLoop();

    // Termination
    PPB.Terminate();
    xcore::Kill();
}












void GlutTimer( int value )
{
    UNREFERENCED_PARAMETER( value );
    glutPostRedisplay();
    glutTimerFunc( 15, GlutTimer, 0 );
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
            sound
        >();
    }

    // Register Systems
    {
        PPB.RegisterSystems<
            physics_system,
            ship_logic_system,
            bullet_logic_system,
            ship_rendering_system,
            bullet_rendering_system,
            buffer_refresh_system,
            imgui_system,
            sound_system,
            scripting_system
        >();
    }

    // Entity Creation
    {
        for (int i = 0; i < 2000; ++i)
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
    }
}