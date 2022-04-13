/**********************************************************************************
*\file         main.cs
*\brief        main.cs Script
*
*\author	     Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			         or disclosure of this file or its contents without the prior
			         written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace CSScript
{
    public class MainApplication
    {
        // Need to return an instance of the class for external calls
        public static MainApplication getInst()
        { return new MainApplication(); }

        public static void Main()
        {
            //while (true)
            //{
            //    ConsoleKeyInfo key = Console.ReadKey();
            //    if (key.Key == ConsoleKey.Enter)
            //        CompileDLL(true);
            //    if (key.Key == ConsoleKey.Escape)
            //        break;
            //}
        }
    }
}
