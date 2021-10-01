using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// For out of c# implementation
using System.Runtime.CompilerServices;

namespace CSScript
{
    public class Test
    {
        public void externaltest()
        {
            if (Inputs.IsKeyPressDown(Inputs.PB_W))
                Camera.MoveForward();
            if (Inputs.IsKeyPressDown(Inputs.PB_S))
                Camera.MoveBackward();
            if (Inputs.IsKeyPressDown(Inputs.PB_A))
                Camera.MoveLeft();
            if (Inputs.IsKeyPressDown(Inputs.PB_D))
                Camera.MoveRight();
            if (Inputs.IsKeyPressDown(Inputs.PB_Q))
                Camera.MoveUp();
            if (Inputs.IsKeyPressDown(Inputs.PB_E))
                Camera.MoveDown();
            if (Inputs.IsKeyPressDown(Inputs.PB_UP))
                Camera.RotateUp();
            if (Inputs.IsKeyPressDown(Inputs.PB_DOWN))
                Camera.RotateDown();
            if (Inputs.IsKeyPressDown(Inputs.PB_LEFT))
                Camera.RotateLeft();
            if (Inputs.IsKeyPressDown(Inputs.PB_RIGHT))
                Camera.RotateRight();
        }
    }
    public class Main
    {
        public static void main()
        {
            //Test.print("C# Main");
        }

        // Need to return an instance of the class for external calls
        public static Test testinst()
        { return new Test(); }
    }
}
