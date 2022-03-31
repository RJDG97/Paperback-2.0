using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Player
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static Int32 GetJumpUnitID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static Int32 GetPushUnitID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static Int32 GetDialogueTextID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TogglePlayers();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static UInt32[] GetAllFreezeable();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static UInt32[] GetAllPushable();
    }
}
