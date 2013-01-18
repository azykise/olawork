using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace scene
{
    using editor;
    class SceneWorld : EditWorld
    {
        public SceneWorld(string name) :
            base(name)
        {

        }

        public override void initialize(EditorDoc parent)
        {
            base.initialize(parent);
            Domain domain = DomainManager.Instance().getDomain(parent);

            //mRenderAABB = new SceneBorderCube("world_render_box", domain.getViewManager().RenderSystem);
            //mRenderAABB.Size = new Vector3F(mWidth, mHeight, mLength);
        }

        public void render(Engine.RenderSystem rendersys)
        {
            //mRenderAABB.render(rendersys);
        }

        SceneBorderCube mRenderAABB = null;
    }
}
