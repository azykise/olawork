using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using editor;

namespace modelview
{
    public partial class ModelView_Form : UserControl
    {
        public const string DOCK_TYPE_BUTTOM = "DOCK_TYPE_BUTTOM";
        public const string DOCK_TYPE_RIGHT_TAB1 = "DOCK_TYPE_RIGHT_TAB1";
        public const string DOCK_TYPE_RIGHT_TAB2 = "DOCK_TYPE_RIGHT_TAB2";

        public ModelView_Form()
        {
            InitializeComponent();
        }

        public virtual void mergeToolBar(System.Windows.Forms.ToolStrip bar)
        {
            ToolStripItem[] items = new ToolStripItem[bar.Items.Count];
            for (int i = 0; i < bar.Items.Count; i++)
            {
                items[i] = bar.Items[i];
            }
            this.toolStrip1.Items.AddRange(items);
        }

        public virtual void insertDockControl(string dock_type,string tab_name,System.Windows.Forms.Control c)
        {
            switch (dock_type)
            {
                case DOCK_TYPE_BUTTOM:
                    this.splitContainer1.Panel2.Controls.Clear();
                    this.splitContainer1.Panel2.Controls.Add(c);
                    c.Dock = DockStyle.Fill;
            	    break;
                case DOCK_TYPE_RIGHT_TAB1:
                    this.tabPage1.Controls.Clear();
                    this.tabPage1.Controls.Add(c);
                    this.tabPage1.Text = tab_name;
                    c.Dock = DockStyle.Fill;
                    break;
                case DOCK_TYPE_RIGHT_TAB2:
                    this.tabPage2.Controls.Clear();
                    this.tabPage2.Controls.Add(c);
                    this.tabPage2.Text = tab_name;
                    c.Dock = DockStyle.Fill;
                    break;
            }
        }

        public void setRightPanelExpand(bool e)
        {
            this.splitContainer2.Panel2Collapsed = !e;
        }

        public HWRenderPanel RenderPanel { get { return hwRenderPanel1; } }
    }
}
