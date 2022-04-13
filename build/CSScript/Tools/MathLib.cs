/**********************************************************************************
*\file         MathLib.cs
*\brief        MathLib.cs Script
*
*\author	     Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript.Tools
{
    public class MathLib
    {
        public struct Vector3
        {
            public float x;
            public float y;
            public float z;

            public Vector3(float a, float b, float c)
            {
                x = a;
                y = b;
                z = c;
            }

            public static Vector3 operator -(Vector3 a, Vector3 b)
                => new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);

            public static Vector3 operator +(Vector3 a, Vector3 b)
            => new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
            public static Vector3 operator *(float a, Vector3 b)
            => new Vector3(a * b.x, a * b.y, a * b.z);

            public float SquaredDistance(Vector3 point_a, Vector3 point_b)
            {
                float a = point_a.x - point_b.x;
                float b = point_a.y - point_b.y;
                float c = point_a.z - point_b.z;

                return a*a + b*b + c*c;
            }
        }
    }
}
