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
            Main.print("Start");
        }
        public void Update()
        {
            Main.print("Update");
        }
        public void Destroy()
        {
            Main.print("Destroy");
        }
    }
}
