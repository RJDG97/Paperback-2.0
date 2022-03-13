#pragma once

#include "Mono.h"
#include "../Sandbox/Systems/SoundSystem.h"

namespace MONO_APPLICATION
{
	MONO_EXPORT void Quit()
	{
		return PPB.QuitApplication();
	}

	MONO_EXPORT void ChangeScene(MonoString* scene_name)
	{

		PPB.QueueScene(mono_string_to_utf8(scene_name));
	}

	MONO_EXPORT void TriggerTaggedSound(MonoString* sound_tag)
	{

		PPB.GetSystem<sound_system>().TriggerTaggedSound(mono_string_to_utf8(sound_tag));
	}

	MONO_EXPORT void TriggerGroupTaggedSound(MonoString* sound_tag)
	{

		PPB.GetSystem<sound_system>().TriggerOneOfGroupTaggedSounds(mono_string_to_utf8(sound_tag));
	}

	MONO_EXPORT void Trigger3DTaggedSound(MonoString* sound_tag, float pos_x, float pos_y, float pos_z, float vel_x, float vel_y, float vel_z, int entity_id)
	{

		PPB.GetSystem<sound_system>().Trigger3DTaggedSound(mono_string_to_utf8(sound_tag), { pos_x, pos_y, pos_z }, { vel_x, vel_y, vel_z }, static_cast<size_t>(entity_id));
	}

	MONO_EXPORT void StopTaggedSound(MonoString* sound_tag)
	{

		PPB.GetSystem<sound_system>().StopTriggeredSoundEvent(mono_string_to_utf8(sound_tag));
	}

	MONO_EXPORT void StopTaggedSoundComp(int sound_tag)
	{

		PPB.GetSystem<sound_system>().StopTriggeredSoundEvent(static_cast<size_t>(sound_tag));
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Application::Quit()", &MONO_APPLICATION::Quit);
		mono_add_internal_call("CSScript.Application::ChangeScene(string)", &MONO_APPLICATION::ChangeScene);
		mono_add_internal_call("CSScript.Application::TriggerTaggedSound(string)", &MONO_APPLICATION::TriggerTaggedSound);
		mono_add_internal_call("CSScript.Application::TriggerGroupTaggedSound(string)", &MONO_APPLICATION::TriggerGroupTaggedSound);
		mono_add_internal_call("CSScript.Application::Trigger3DTaggedSound(string,single,single,single,single,single,single,uint)", &MONO_APPLICATION::Trigger3DTaggedSound);
		mono_add_internal_call("CSScript.Application::StopTaggedSound(string)", &MONO_APPLICATION::StopTaggedSound);
		mono_add_internal_call("CSScript.Application::StopTaggedSoundComp(uint)", &MONO_APPLICATION::StopTaggedSoundComp);
	}

}
