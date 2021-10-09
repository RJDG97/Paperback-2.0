#include "PhysicsSystem.h"
#include "CollisionSystem.h"          // Physics - ?? - DrawSphere / DrawCube shouldnt be in here....
#include "ImguiSystem.h"
#include "SoundSystem.h"                // Banks should be loaded from json or sth - Replace couts
#include "ScriptingSystem.h"            // I do not think u need to call delete on the ptr - It will probably crash tbh - Replace couts in singleton
#include "WindowSystem.h"               // Replace couts - Clean comments
#include "DebugSystem.h"                // Verify if we need it
#include "RendererSystem.h"             // Remove commented code
#include "AnimatorSystem.h"