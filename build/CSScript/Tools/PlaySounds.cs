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
