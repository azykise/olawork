using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Sharp3D.Math.Core;

namespace scene.ui
{
    using editor;
    partial class UICreatePrimitive : UserControl
    {

        public UICreatePrimitive()
        {
            InitializeComponent();
            //this.comboBoxCreateItem.Items.Add(SceneBorderCube.CLASS_NAME);
            //this.comboBoxCreateItem.Items.Add(ScenePlane.CLASS_NAME);
        }

        public SceneEntityManager EntityMng { set { mEntityMng = value; } }
        SceneEntityManager mEntityMng = null;

        string mCreateItemName = "";
        Dictionary<string,object> mCreatePropertys = new Dictionary<string,object>();

        private void comboBoxCreateItem_SelectedValueChanged(object sender, EventArgs e)
        {
        //    mCreateItemName = this.comboBoxCreateItem.SelectedItem.ToString();
        //    refreshCreatePropertys();
        }

        private void refreshCreatePropertys()
        {
            //this.propertyGridCreateProperty.Item.Clear();
            //EntityMemento memento = null;
            //switch(mCreateItemName)
            //{
            //    case SceneBorderCube.CLASS_NAME:
            //        memento = new SceneBorderCubeMemento(null);
            //        break;
            //    case ScenePlane.CLASS_NAME:
            //        memento = new ScenePlaneMemento(null);
            //        break;
            //    default: return;
            //}

            //if (memento == null)
            //{
            //    return;
            //}

            //this.propertyGridCreateProperty.Visible = false;
            //EntityPropertyCooker.entityMementoToGrid(memento, this.propertyGridCreateProperty);
            //this.propertyGridCreateProperty.Visible = true;
        }

        private void buttonOnCreate_Click(object sender, EventArgs e)
        {
            //Domain domain = DomainManager.Instance().getDomain(mEntityMng);
            //Engine.RenderSystem rendersys = domain.getViewManager().RenderSystem;

            //Entity entity = mEntityMng.PrimitiveFactory.createEntity(mCreateItemName, mCreatePropertys, rendersys);

            //if (entity != null)
            //{
            //    mEntityMng.CurrentEntityLayer.OriginBuffer.insert(entity);
            //    EditorGraph.broadCast(domain, mEntityMng, new CastMessage(MessageType.Editor, CommonEntityNode.EntityCreated));
            //}
        }

        private void propertyGridCreateProperty_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
//           EntityPropertyCooker.gridPropertyToPropertyDic(e.ChangedItem, mCreatePropertys);
        }
    }
}
