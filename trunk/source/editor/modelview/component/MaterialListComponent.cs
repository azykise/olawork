using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace modelview
{
    using editor;
    class MaterialListComponent : EditorComponent
    {
        public MaterialListComponent(ModelViewDoc doc) :
            base("MaterialListComponent")
        {
            Doc = doc;

            MaterialListUI = new modelview.ui.ModelMatList();
            MaterialListUI.EditorComponent = this;

            registMessage("AfterModelViewUICreated", onAfterModelViewUICreatedMessage);

            registMessage("AfterModelCreated", onAfterModelCreatedMessage);
        }

        private void onAfterModelCreatedMessage(EditorComponent from, MessageBase msg)
        {
            updateUI();
        }

        public void updateUI()
        {
            if (Doc.CurrentStaticModel == null)
            {
                return;
            }
            mMaterials.Clear();
            
            string dmlfilename = Doc.CurrentStaticModel.Name;
            DmlInfo dml = new DmlInfo();
            dml.fromXML(dmlfilename);
            MaterialListUI.initializeUI(dml);
        }

        private void onAfterModelViewUICreatedMessage(EditorComponent from, MessageBase msg)
        {
            //refreshActionList();
        }

        List<MaterialInfo> mMaterials = new List<MaterialInfo>();

        public ModelViewDoc Doc = null;
        public modelview.ui.ModelMatList MaterialListUI = null;
    }
}
