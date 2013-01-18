using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;

namespace scene
{
    class SceneEditorDoc : EditorDoc
    {
        public SceneEditorDoc(int num) :
            base("SceneEditorDoc_" + num.ToString())
        {

        }

        protected override void initialize()
        {
            if (mEntityManager != null)
            {
                mEntityManager.initialize();
            }

            mWorld.initialize(this);

            EditorGraph.broadCast(this, new MessageBase(MessageType.Doc,SceneEditorNote.DocInitDone));
        }

        public override EditWorld World { get { return mWorld; } }

        protected SceneWorld mWorld = new SceneWorld("sceneworld");
    }

    enum SceneEditorNote : int
    {
        UnKnown = (int)default(SceneEditorNote),
        DocInitDone = 1001,       
    }
}
