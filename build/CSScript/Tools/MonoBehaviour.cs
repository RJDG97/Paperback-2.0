using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public interface MonoBehaviour
    {
        void Start();
        void Update(float dt);
        void Destroy();
    }
}
