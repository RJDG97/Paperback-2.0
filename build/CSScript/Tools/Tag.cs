/**********************************************************************************
*\file         Tag.cs
*\brief        Tag.cs Script
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

namespace CSScript.Tools
{
    public class Tag
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsPushable(UInt32 ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool IsFreezable(UInt32 ID);
    }
}
