using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    public abstract class Common3DView : View
    {
        public Common3DView(string name, RenderSystem rs):
            base(name,rs)
        {

        }
    }
}
