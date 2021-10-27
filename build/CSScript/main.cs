using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

using System.CodeDom.Compiler;
using Microsoft.CSharp;

namespace CSScript
{
    public class Main
    {
        // Need to return an instance of the class for external calls
        public static Main getInst()
        { return new Main(); }

        public static void CompileDLL()
        {
            string[] references = {
                "System.dll",
                "Microsoft.CSharp.dll"
            };

            string[] files = {
                "main.cs",
                "QuitScript.cs",
                "Unit.cs",
                "CameraMovement.cs",
                "Application.cs",
                "Camera.cs",
                "Debug.cs",
                "Inputs.cs",
                "MonoBehaviour.cs"
            };

            CSharpCodeProvider provider = new CSharpCodeProvider();
            CompilerParameters parameters = new CompilerParameters(references);

            // Set compilation output to dll (true = exe, false = dll)
            parameters.GenerateExecutable = false;
            // Output Name
            parameters.OutputAssembly = "Scriptinglib.dll";
            // Save as dll
            parameters.GenerateInMemory = false;
            // Generate DLL
            var result = provider.CompileAssemblyFromFile(parameters, files);

            if (result.Errors.Count != 0)
            {
                foreach (CompilerError error in result.Errors)
                    Console.WriteLine(error.ErrorText);
            }
            else
                Console.WriteLine("Compile Successful");
        }

        public static void main()
        {
        }
    }
}
