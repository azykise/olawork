using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace modelview
{
    using editor;
    class ToolBarCommonComponent : EditorComponent
    {
        public ToolBarCommonComponent(ModelViewDoc doc):
            base("ToolBarCommonComponent")
        {
            Doc = doc;

            ToolbarUI = new modelview.ui.ToolBarCommon();
            ToolbarUI.EditorComponent = this;
        }

        public void reloadCurrentModel()
        {
            if (Doc.CurrentStaticModel == null)
                return;

            //Doc.CurrentStaticModel.reloadMaterials();

            Doc.CurrentStaticModel.reloadDml();
        }

        public void openCurrentModelFolder()
        {
            if (Doc.CurrentStaticModel == null)
                return;

            string abs_filename = Tool.GetAssetAbsoultFilePath(Doc.CurrentStaticModel.Name);
            string abs_path = Tool.GetFilePath(abs_filename);
            System.Diagnostics.Process.Start("Explorer.exe", abs_path);
        }

        public void setRightPanelExpand(bool expand)
        {
            EditorGraph.postMessage(this, new MessageBase("ExpandRightPanel", expand.ToString().ToLower()));
        }

        public ModelViewDoc Doc = null;
        public modelview.ui.ToolBarCommon ToolbarUI = null;
    }
}
