using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Camera {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveForward();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveBackward();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveLeft();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveRight();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveUp();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void MoveDown();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateUp();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateDown();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateLeft();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateRight();
    }
}
