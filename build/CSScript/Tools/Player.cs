﻿using System;
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
        public extern static Int32 GetRedRobotID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static Int32 GetBlueRobotID();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void TogglePlayers();
    }
}
