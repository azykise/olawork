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
    class EntityListComponent : EditorComponent
    {
        public EntityListComponent(EntityList ui) :
            base(typeof(EntityListComponent).ToString())
        {
            mUI = ui;
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            switch (msg.MsgType)
            {
                case MessageType.Doc:
                    switch (msg.getNote<SceneEditorNote>())
                    {
                        case SceneEditorNote.DocInitDone:
                            mUI.initialize();
                            break;
                    }
                    break;
                case MessageType.NewDoc:
                    mUI.initialize();
                    break;
                case MessageType.Editor:
                    switch (msg.getNote<CommonEntityNode>())
                    {
                        case CommonEntityNode.EntityCreated:
                            mUI.refreshEntityTree(null);
                            break;
                    }
                    break; ;
            }
            return NotifyResult.Pending;
        }

        public EntityList UI { get { return mUI; } }

        EntityList mUI;
    }
}