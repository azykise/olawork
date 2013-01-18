using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace editor
{
    static class Program
    {
        static Program()
        {

        }
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Editor_Form main_form = new Editor_Form();
            main_form.Show();
            main_form.initEditor(System.Environment.GetCommandLineArgs());

            Application.Run(main_form);
        }
    }
}
