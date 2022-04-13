/**********************************************************************************
*\file         PlaySounds.cs
*\brief        PlaySounds.cs Script
*
*\author	     Bryan Low, 100% Code Contribution
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
    public class PlaySounds
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TriggerGroupTaggedSound(string sound_tag);
    }
}
