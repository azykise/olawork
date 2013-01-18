using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using editor;

namespace scene
{
    public partial class SceneEditor_Form : UserControl, editor.ui.IDockableWindow
    {
        public SceneEditor_Form()
        {
            InitializeComponent();
        }

        public virtual void mergeToolBar(System.Windows.Forms.ToolStrip bar)
        {
            //DevComponents.DotNetBar.BaseItem[] items = new DevComponents.DotNetBar.BaseItem[bar.Items.Count];

            ToolStripItem[] items = new ToolStripItem[bar.Items.Count];
            for (int i = 0; i < bar.Items.Count; i++)
            {
                items[i] = bar.Items[i];
            }
            this.toolStripMain.Items.AddRange(items);
        }

        public virtual void insertDockControl(editor.ui.DockTo dockto, System.Windows.Forms.Control c)
        {
            SceneEditorDockTo _to = dockto as SceneEditorDockTo;

            switch (_to.mDockto)
            {
                case SceneEditor_FormDockID.CreateDock:
                    this.tabPageCreate.Controls.Clear();
                    this.tabPageCreate.Controls.Add(c);
                    c.Dock = DockStyle.Fill;
                    break;
                case SceneEditor_FormDockID.PropertyDock:
                    this.tabPageProperty.Controls.Clear();
                    this.tabPageProperty.Controls.Add(c);
                    c.Dock = DockStyle.Fill;
                    break;
                case SceneEditor_FormDockID.ListDock:
                    this.tabPageList.Controls.Clear();
                    this.tabPageList.Controls.Add(c);
                    c.Dock = DockStyle.Fill;
                    break;

            }
        }

        public HWRenderPanel RenderPanel { get { return this.hwRenderPanel1; } }
    }

    class SceneEditorDockTo : editor.ui.DockTo
    {
        public SceneEditorDockTo(SceneEditor_FormDockID dockto)
        {
            mDockto = dockto;
        }
        public SceneEditor_FormDockID mDockto;
    }

    enum SceneEditor_FormDockID
    {
        CreateDock,
        PropertyDock,
        ListDock,
    }
}
