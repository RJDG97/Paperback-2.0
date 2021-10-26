using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class QuitScript : MonoBehaviour
    {
        public static QuitScript getInst()
        {
            return new QuitScript();
        }
        public void Start()
        {
        }
        public void Update(float dt)
        {
            //Console.WriteLine(dt);
            if (Input.IsKeyPress(Input.PB_ESCAPE))
                Application.Quit();
        }
        public void Destroy()
        {
        }
    }
}
