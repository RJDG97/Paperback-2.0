/**********************************************************************************
*\file         Debug.cs
*\brief        Debug.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
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
    public class Debug
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Log(string str);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern static void DrawLine(vector3 begin ,vector3 end, Color colour);
    }
}
