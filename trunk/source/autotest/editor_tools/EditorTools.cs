using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace tframework
{
    public class CaseGlobal
    {
        static CaseGlobal()
        {
            CaseGlobal.sEditorForm = new editor.Editor_Form();
            string[] mock_config = { "ooxx" };
            CaseGlobal.sEditorForm.initEditor(mock_config);
            CaseGlobal.sEditorForm.Hide();
        }

        public static editor.Editor_Form sEditorForm = null;
    }

    public class EditorTools
    {
        public static T GetSubControlByName<T>(Control parent,string name)
        {
            object c = FindSubControlByName(parent, name);
            return c == null && c is T ? (T)c : default(T);
        }

        private static Control FindSubControlByName(Control parent, string name)
        {
            if (parent.Name == name)
            {
                return parent;
            }

            foreach (Control sub_c in parent.Controls)
            {
                Control fc = FindSubControlByName(sub_c, name);
                if (fc != null)
                {
                    return fc;
                }
            }

            return null;
        }
    }
}
