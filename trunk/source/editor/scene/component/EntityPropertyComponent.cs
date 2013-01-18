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
    class EntityPropertyComponent : EditorComponent
    {
        public EntityPropertyComponent(EntityPropertyGrid ui) :
            base("EntityProperty")
        {
            mUI = ui;
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            switch (msg.MsgType)
            {
                case MessageType.Editor:
                    Domain domain = null;
                    switch (msg.getNote<SceneEditorNote>())
                    {
                        case SceneEditorNote.DocInitDone:
                            domain = DomainManager.Instance().getDomain(this);
                            mUI.EntityMng = domain.getCurrentDoc().EntityMng as SceneEntityManager;
                            break;

                    }
                    switch (msg.getNote<CommonEntityNode>())
                    {
                        case CommonEntityNode.SelectedEntityChanged:
                            domain = DomainManager.Instance().getDomain(this);
                            mUI.EntityMng = domain.getCurrentDoc().EntityMng as SceneEntityManager;
                            mUI.refreshPropertyGrid();
                            break; 
                    }
                    break;
            }
            return NotifyResult.Pending;
        }

        EntityPropertyGrid mUI;
    }
}