using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;

namespace modelview
{
    class BoneListComponent : EditorComponent
    {
        public BoneListComponent(ModelViewDoc doc) :
            base("BoneListComponent")
        {
            Doc = doc;

            BoneListUI = new modelview.ui.BoneList();
            BoneListUI.EditorComponent = this;

            registMessage("AfterModelCreated", onAfterModelCreatedMessage);
        }

        private void onAfterModelCreatedMessage(EditorComponent from, MessageBase msg)
        {
            if (Doc.CurrentDynamicModel != null)
            {
                BoneListUI.updateBoneList(Doc.CurrentDynamicModel.RootBone);
            }
            else
                BoneListUI.updateBoneList(null);
            
        }

        public ModelViewDoc Doc = null;
        public modelview.ui.BoneList BoneListUI = null;
    }
}
