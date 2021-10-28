using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

using System.CodeDom.Compiler;
using Microsoft.CSharp;

namespace CSScript
{
    public class MainApplication
    {
        // Need to return an instance of the class for external calls
        public static MainApplication getInst()
        { return new MainApplication(); }

        public static void CompileDLL(bool Useingdll_1)
        {
            string[] references = {
                "System.dll"
            };

            string[] files = {
                @"..\CSScript\main.cs",
                @"..\CSScript\QuitScript.cs",
                @"..\CSScript\Unit.cs",
                @"..\CSScript\CameraMovement.cs",
                @"..\CSScript\Tools\Application.cs",
                @"..\CSScript\Tools\Camera.cs",
                @"..\CSScript\Tools\Debug.cs",
                @"..\CSScript\Tools\Inputs.cs",
                @"..\CSScript\Tools\MonoBehaviour.cs"
            };

            CSharpCodeProvider provider = new CSharpCodeProvider();
            CompilerParameters parameters = new CompilerParameters(references);

            // Set compilation output to dll (true = exe, false = dll)
            parameters.GenerateExecutable = false;
            // Output Name
            if (Useingdll_1)
                parameters.OutputAssembly = @"CSScript2.dll";
            else
                parameters.OutputAssembly = @"CSScript.dll";
            // Save as dll
            parameters.GenerateInMemory = true;
            // Generate DLL
            var result = provider.CompileAssemblyFromFile(parameters, new string[] { });

            if (result.Errors.Count != 0)
            {
                foreach (CompilerError error in result.Errors)
                    Debug.Log(error.ErrorText);
            }
            else
                Debug.Log("Compile Successful");
        }

        public static void Main()
        {
            while (true)
            {
                ConsoleKeyInfo key = Console.ReadKey();
                if (key.Key == ConsoleKey.Enter)
                    CompileDLL(true);
                if (key.Key == ConsoleKey.Escape)
                    break;
            }
        }
    }
}
