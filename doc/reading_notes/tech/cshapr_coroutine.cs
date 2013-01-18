using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace csharp_coroutine
{
    class Program
    {        
        static IEnumerable<int> DoCoroutine()
        {
            int i = 0;

            while(true)
            {                
                System.Console.WriteLine((i++).ToString());
                yield return i;
            }
        }

        static void Main(string[] args)
        {
            IEnumerator<int> co = DoCoroutine().GetEnumerator();

            co.MoveNext();
            co.MoveNext();
            co.MoveNext();

            System.Console.ReadLine();
        }
    }
}
