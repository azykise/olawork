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
    partial class EntityCreate : UserControl
    {
        public EntityCreate()
        {
            InitializeComponent();
            mComponent = new EntityCreateComponent(this);
        }

        ui.UICreatePrimitive mCreatePrimitive = new ui.UICreatePrimitive();
        EntityCreateComponent mComponent = null;

        public EntityCreateComponent EditorComponent { get { return mComponent; } }

        private void buttonCreatePrim_Click(object sender, EventArgs e)
        {
            //this.panelCreateDock.Controls.Clear();
            //this.panelCreateDock.Controls.Add(mCreatePrimitive);
            //mCreatePrimitive.Dock = DockStyle.Fill;

            //Domain domain = DomainManager.Instance().getDomain(mComponent);
            //EditorDoc doc = domain.getCurrentDoc();
            //SceneEntityManager entity_mng = doc.EntityMng as SceneEntityManager;
            //mCreatePrimitive.EntityMng = entity_mng;
        }

        private void expandPanel1_OnExpandChange(bool expand)
        {
            this.splitContainer1.Panel1Collapsed = !expand;
        }
    }
}
