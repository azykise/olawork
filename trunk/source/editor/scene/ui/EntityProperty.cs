using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace scene
{
    using editor;
    using scene;
    using Sharp3D.Math.Core;
    partial class EntityPropertyGrid : UserControl
    {
        public EntityPropertyGrid()
        {
            InitializeComponent();

            mComponent = new EntityPropertyComponent(this);
        }

        public EditorComponent EditorComponent { get { return mComponent; } }
        EntityPropertyComponent mComponent = null;

        public Entity CurrentEntity { get { return mCurrentEntity; } set { mCurrentEntity = value; } }
        Entity mCurrentEntity = null;

        public SceneEntityManager EntityMng { get { return mEntityMng; } set { mEntityMng = value; } }
        SceneEntityManager mEntityMng = null;

        public void refreshPropertyGrid()
        {
            //mCurrentEntity = null;
            //this.propertyGridEntity.Item.Clear();

            //if (mEntityMng != null && mEntityMng.CurrentEntityLayer.SelectBuffer.count > 0)
            //{
            //    mCurrentEntity = mEntityMng.CurrentEntityLayer.SelectBuffer.first();

            //    EntityPropertyCooker.entityMementoToGrid(mCurrentEntity.getMemento(), this.propertyGridEntity);
            //}

            //this.propertyGridEntity.Refresh();
        }

        private void propertyGridEntity_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            //if (mCurrentEntity == null)
            //{
            //    return;
            //}

            //EntityMemento memento = mCurrentEntity.getMemento();

            //EntityPropertyCooker.GridItemToEntityMemento(e.ChangedItem, memento);

            //memento.setToEntity();
        }
    }
}
