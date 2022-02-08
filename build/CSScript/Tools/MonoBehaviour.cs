using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public interface MonoBehaviour
    {
        void Start(UInt32 ID);
        void Update(float dt);
        void Destroy();

        void OnCollisionEnter(UInt32 ID);
        void OnCollisionStay(UInt32 ID);
        void OnCollisionExit(UInt32 ID);
    }
}
