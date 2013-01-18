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
    class EntityCreateComponent : EditorComponent
    {
        public EntityCreateComponent(EntityCreate ui) :
            base("EntityCreateComponent")
        {
            mUI = ui;
        }

        private void onMessageEntityCreated(EditorComponent from, MessageBase msg)
        {

        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Pending;
        }

        EntityCreate mUI = null;
    }
}