using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor.ui
{
    using editor;
    public partial class ToolBarDocControl : UserControl 
    {
        public const string OPEN_FILE_MESSAGE = "ToolBarDocControl_OpenFile";

        public ToolBarDocControl(string workarea)
        {
            InitializeComponent();
            m_workarea = workarea;
            m_editcomp = new ToolBarDocComponent(this);
        }

        string m_workarea = "";
        public EditorComponent EditorComponent { get { return m_editcomp; } }
        public System.Windows.Forms.ToolStrip ToolBar { get { return this.toolStrip1; } }

        private void buttonNewDoc_Click(object sender, EventArgs e)
        {
            
        }

        private void buttonOpenDoc_Click(object sender, EventArgs e)
        {
            EditorGraph.postMessage(m_editcomp, new MessageBase(OPEN_FILE_MESSAGE));
        }

        private void buttonDocSave_Click(object sender, EventArgs e)
        {
            
        }

        public class ToolBarDocComponent : EditorComponent
        {
            public ToolBarDocComponent(ToolBarDocControl bar) :
                base(bar.m_workarea +  "_ToolBarDocComponent")
            {
                m_bar = bar;
            }

            public override NotifyResult notified(EditorComponent from, MessageBase msg)
            {
                return NotifyResult.Pending;
            }

            ToolBarDocControl m_bar = null;
        }
        ToolBarDocComponent m_editcomp = null;
    }


}
