using System;
using System.Collections.Generic;
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
            if (Input.IsKeyPress(Input.PB_ENTER))
                Debug.Log("Test");
            if (Input.IsKeyPress(Input.PB_ESCAPE))
                Application.Quit();
        }
        public void Destroy()
        {
        }
    }
}
