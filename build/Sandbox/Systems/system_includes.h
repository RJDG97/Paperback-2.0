#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "ImguiSystem.h"
#include "SoundSystem.h"
#include "ScriptingSystem.h"
#include "WindowSystem.h"
#include "DebugSystem.h"
#include "RendererSystem.h"
#include "AnimatorSystem.h"
#include "ParentChildSystem.h"

#include "TimerSystem.h"
#include "EnemySpawnerSystem.h"
#include "PlayerSpawnerSystem.h"
#include "WaypointSystem.h"
#include "HealthSystem.h"
#include "UISystem.h"
#include "DeckSystem.h"

// Event
#include "OnEvent_CollisionSystem.h"
#include "OnEvent_ParentOrChildDeleted.h"
#include "OnEvent_HealthSystem.h"
#include "OnEvent_ReferencePrefabDeleted.h"
