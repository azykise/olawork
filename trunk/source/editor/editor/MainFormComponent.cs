using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    class MainFormComponent : EditorComponent
    {
        public MainFormComponent(Editor_Form form) :
            base("MainForm")
        {
            mForm = form;
        }

        Editor_Form mForm = null;

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        { 
            return NotifyResult.Succeed;
        }
    }
}
