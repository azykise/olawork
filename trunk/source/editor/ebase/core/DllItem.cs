using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

//DllItem：非托管的DLL对象
//动态加载非托管的DLL对象，需要先静态加载kernel32.dll

/*  使用方法：
 *  "在xxx.dll中的函数 : int fun(void* p);"
 *  DllItem dll = new DllItem("xxx.dll");
 *  delegate int xxxFun(IntPtr p);
 *  xxxFun fun = dll.getFunction<xxxFun>("fun");
 *  if(fun != null)
 *  {
 *      IntPtr p = new IntPtr(xxx..);
 *      int n = fun(p);
 *  }
 */
namespace editor
{
    public class DllItem : IDisposable
    {
        [DllImport("Kernel32.dll")]
        static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern int GetProcAddress(IntPtr hModule, string lpProcName);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary", SetLastError = true)]
        static extern bool FreeLibrary(IntPtr hModule);

        public DllItem(string filename)
        {
            loadDll(filename);
        }

        ~DllItem()
        {
            Dispose();
        }

        public bool loadDll(string filename)
        {
            if (enable)
                return false;

            m_dll = LoadLibrary(filename);

            if (enable)
                return true;
            else
            {
                m_dll = new IntPtr(0);
                return false;
            }
        }

        public void Dispose()
        {
            if (enable)
            {
                if (FreeLibrary(m_dll))
                {
                    m_dll = new IntPtr(0);
                }
            }
        }

        public bool enable
        {
            get { return m_dll.ToInt32() != 0; }
        }

        public T getFunction<T>(string functionname)
        {
            if (!enable)
                return default(T);

            int addr = GetProcAddress(m_dll, functionname);

            return addr != 0 ? (T)(Object)Marshal.GetDelegateForFunctionPointer(new IntPtr(addr), typeof(T)) : default(T);
        }

        IntPtr m_dll = new IntPtr(0);
    }
}
