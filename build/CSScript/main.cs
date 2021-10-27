using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

using System.CodeDom.Compiler;
using Microsoft.CSharp;
using Mono.CSharp;
using Mono.CompilerServices.SymbolWriter;
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
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\main.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\QuitScript.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Unit.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\CameraMovement.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Tools\Application.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Tools\Camera.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Tools\Debug.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Tools\Inputs.cs",
                @"C:\Users\Renzo\Documents\School\Fall '21\GAM300\Paperback-2.0\build\CSScript\Tools\MonoBehaviour.cs"
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
