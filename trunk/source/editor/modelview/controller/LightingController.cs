using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using Sharp3D.Math.Core;

namespace modelview
{
    class LightingController : Controller
    {
        public LightingController(ModelViewDoc doc):
            base("LightingController")
        {
            mDoc = doc;
        }

        public void moveingLight(Vector3F src,Vector3F dst)
        {
            mDoc.DirLight0.SrcPos = src;
            mDoc.DirLight0.DstPos = dst;
        }

        ModelViewDoc mDoc = null;
    }
}
