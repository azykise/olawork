using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using editor;

namespace modelview
{
    class ModelViewDoc : EditorDoc
    {
        public ModelViewDoc(ModelView domain):
            base("ModelViewDoc")
        {
            Domain = domain;
            this.registMessage(editor.ui.ToolBarDocControl.OPEN_FILE_MESSAGE, onOpenFileMessage);

            ActionMng = new ActionManager(this);
        }

        public override void release()
        {
            close();

            if (Grid != null)
            {
                Grid.Dispose();
                Grid = null;
            }

            if (DirLight0 != null)
            {
                DirLight0.Dispose();
                DirLight0 = null;
            }

            if (CurrentScene != null)
            {
                CurrentScene.Dispose();
                CurrentScene = null;
            }

            base.release();
        }

        protected override void initialize()
        {
            if(Grid == null)
                Grid = Domain.getViewManager().RenderSystem.createPrimitiveGrid(new Vector3F(0, 0, 0), 100, 100, 5, 5);

            DirLight0 = Domain.getViewManager().RenderSystem.createLight("DirLight0");
            DirLight0.SrcPos = new Vector3F(200.0f, 200.0f, 200.0f);
            DirLight0.DstPos = new Vector3F(0, 0, 0);

            CurrentScene = Domain.getViewManager().RenderSystem.createSmallScene("ModelViewScene");
            DirLight0.attach(CurrentScene);

            Domain.getViewManager().RenderSystem.setRenderScene(CurrentScene);
        }

        private void onOpenFileMessage(EditorComponent from, MessageBase msg)
        {
            string file_name = "";

            if (msg.Messages.Count != 0)
            {
                file_name = msg.Messages[0];
            }
            else
            {
                System.Windows.Forms.OpenFileDialog of = new System.Windows.Forms.OpenFileDialog();
                of.Title = "打开...";
                of.ShowReadOnly = true;
                of.Filter = "Model Files(*.dml;*.chr)|*.dml;*.chr";
                of.FilterIndex = 1;
                of.RestoreDirectory = true;
                of.InitialDirectory = "../assets/";
                file_name = of.FileName;
                if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    file_name = of.FileName;
                }
            }

            open(file_name);                          
        }

        public override bool open(string filename)
        {
            if (filename.Length == 0)
                return false;

            string s = Tool.GetAssetsRelativeFileFullPath(filename);
            string suffix = Tool.GetFileSuffix(s);

            bool b = false;
            switch (suffix)
            {
                case Tool.SUFFIX_DML:
                    if (CurrentDynamicModel != null)
                        CurrentDynamicModel.Dispose();
                    if (CurrentStaticModel != null)
                        CurrentStaticModel.Dispose();

                    CurrentDynamicModel = null;
                    CurrentStaticModel = Domain.getViewManager().RenderSystem.createStaticModel(s);
                    b = true;
                    break;
                case Tool.SUFFIX_CHR:
                    if (CurrentDynamicModel != null)
                        CurrentDynamicModel.Dispose();
                    if (CurrentStaticModel != null)
                        CurrentStaticModel.Dispose();
                    CurrentStaticModel = null;
                    CurrentDynamicModel = Domain.getViewManager().RenderSystem.createDynamicModel(s);
                    b = true;
                    break;
            }

            if (b)
            {
                EditorGraph.postMessage(this, new MessageBase("AfterModelCreated"));
            }

            return b;
        }

        public override void close()
        {
            if (CurrentStaticModel != null)
            {
                CurrentStaticModel.Dispose();
                CurrentStaticModel = null;
            }

            if (CurrentDynamicModel != null)
            {
                CurrentDynamicModel.Dispose();
                CurrentDynamicModel = null;
            }

            Domain.getViewManager().RenderSystem.clearResourceCache("");
        }

        public ActionManager ActionMng = null;

        public ModelView Domain = null;
        public Engine.Scene CurrentScene = null;
        public Engine.StaticModel CurrentStaticModel = null;
        public Engine.DynamicModel CurrentDynamicModel = null;
        public Engine.PrimitiveGrid Grid = null;
        public Engine.Light DirLight0 = null;
    }
}
