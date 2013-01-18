using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace modelview.ui
{
    using editor;
    partial class ToolBarCommon : UserControl
    {
        public ToolBarCommon()
        {
            InitializeComponent();
        }

        ToolBarCommonComponent m_editcomp = null;

        public System.Windows.Forms.ToolStrip ToolBar { get { return toolStrip1; } }
        public ToolBarCommonComponent EditorComponent 
        {
            set { m_editcomp = value; }
            get { return m_editcomp; } 
        }

        private void ButtonReloadModel_Click(object sender, EventArgs e)
        {
            m_editcomp.reloadCurrentModel();
        }

        private void ButtonResPath_Click(object sender, EventArgs e)
        {
            m_editcomp.openCurrentModelFolder();
        }

        private void ButtonExpandRight_Click(object sender, EventArgs e)
        {
            this.ButtonExpandRight.Checked = !this.ButtonExpandRight.Checked;
            m_editcomp.setRightPanelExpand(this.ButtonExpandRight.Checked);
        }
    }
}
