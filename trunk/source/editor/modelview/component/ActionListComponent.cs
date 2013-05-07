using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using System.IO;

namespace modelview
{
    class ActionListComponent : EditorComponent
    {
        public ActionListComponent(ModelViewDoc doc) :
            base("ActionListComponent")
        {
            Doc = doc;

            ActionListUI = new modelview.ui.ActionList();
            ActionListUI.EditorComponent = this;

            registMessage("AfterModelViewUICreated", onAfterModelViewUICreatedMessage);
        }

        public void setCurrentAction(string action_name)
        {
            //if (Doc.CurrentDynamicModel != null)
            //{
            //    if (!ActionFiles.ContainsKey(action_name) && Doc.CurrentDynamicModel.DefActionName != action_name)
            //    {
            //        return;
            //    }
                    
            //    string action_filename = ActionFiles[action_name];
            //    Doc.CurrentDynamicModel.setAction(action_filename);
            //    updateCurrentActionName();

            //    ActionListUI.updateUI();
            //    EditorGraph.postMessage(this, new MessageBase("AfterModelActionChanged"));
            //}            
        }

        public void resetCurrentAction()
        {
            //if (Doc.CurrentDynamicModel != null)
            //{
            //    setCurrentAction(Doc.CurrentDynamicModel.DefActionName);
            //}
        }

        public void refreshActionList()
        {
            System.IO.DirectoryInfo dir = new System.IO.DirectoryInfo("../assets/art/role/action");
            if (dir.Exists)
            {
                FileInfo[] files = dir.GetFiles("*.ase");

                ActionFiles.Clear();
                foreach (FileInfo file in files)
                {
                    string fullname = Tool.GetAssetsRelativeFileFullPath(file.FullName);
                    string action_name = file.Name.Substring(0, file.Name.Length - 4);
                    ActionFiles[action_name] = fullname;
                }
            }

            updateCurrentActionName();

            ActionListUI.updateUI();
        }

        private void updateCurrentActionName()
        {
            //if (Doc.CurrentDynamicModel != null && Doc.CurrentDynamicModel.CurrentAction.Name != Doc.CurrentDynamicModel.DefActionName)
            //{
            //    CurrentActionName = Doc.CurrentDynamicModel.CurrentAction.Name;
            //}
            //else
            //    CurrentActionName = null;
        }

        private void onAfterModelViewUICreatedMessage(EditorComponent from, MessageBase msg)
        {
            refreshActionList();
        }

        public Dictionary<string, string> ActionFiles = new Dictionary<string, string>();
        public string CurrentActionName = "";

        public ModelViewDoc Doc = null;
        public modelview.ui.ActionList ActionListUI = null;
    }
}
