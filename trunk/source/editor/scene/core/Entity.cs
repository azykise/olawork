using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;

namespace scene
{    
    class SceneEntityManager : EntityManager 
    {
        public SceneEntityManager():
            base("SceneEntityManager")
        {
            
        }

        public override void initialize()
        {
            base.initialize();
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Pending;
        }

        public PrimitiveFactory PrimitiveFactory { get { return mPrimitiveFactory; } }

        PrimitiveFactory mPrimitiveFactory = new PrimitiveFactory();
    }
}
