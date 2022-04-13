/**********************************************************************************
*\file         Application.cs
*\brief        Application.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    class Application
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Quit();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void ChangeScene(string scene_name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TriggerTaggedSound(string sound_tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TriggerGroupTaggedSound(string sound_tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Trigger3DTaggedSound(string sound_tag, float pos_x, float pos_y, float pos_z, float vel_x, float vel_y, float vel_z, UInt32 entity_id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void StopTaggedSound(string sound_tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void StopTaggedSoundComp(UInt32 sound_tag);
    }
}
