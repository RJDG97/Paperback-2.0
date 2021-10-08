using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    class Unit
    {
        public static Unit getInst()
        {
            return new Unit();
        }
        public void Start()
        {
        }
        public void Update()
        {
            if (Inputs.IsKeyPress(Inputs.PB_X))
                Main.print("Test Script");
            CameraControls.cameracontrols();
        }
        public void Destroy()
        {
        }
    }
}
