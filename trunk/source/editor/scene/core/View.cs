using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace scene
{
    class MainView : Common3DView
    {
        public MainView(Engine.RenderSystem rs) :
            base("SceneMainView", rs)
        {
            //m_grid = rs.createPrimitiveGrid(new Vector3F(0, 0, 0), 10, 10, 50, 50);

            m_camera.Position = new Vector3F(122.31f, 82.37f, 9.61f);
            m_camera.Direction = new Vector3F(73.37f, -61.42f, -21.68f);
            BaseController = new Camera3DController("cam");
            BaseController.DefaultSubController = new EntitySelectDragController();
            BaseController.CurrentSubController = BaseController.DefaultSubController;
        }

        public override void render()
        {
            m_camera.doUpdate(m_render_sys);

            m_window.setBackgroundColor(0, 0, 1, 0);
            m_render_sys.setRenderWindow(m_window);
            

            if (mEntityMng != null)
            {
                mEntityMng.update(0);
                mEntityMng.render(RenderSystem);
            }

           // m_grid.render(m_render_sys);

            BaseController.render();

            if (mWorld != null)
            {
                mWorld.render(RenderSystem);                    
            }

            m_render_sys.beginDraw();
            m_render_sys.drawSceneActors();
            m_render_sys.endDraw();
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            switch (msg.MsgType)
            {
                case MessageType.Doc:
                    switch (msg.getNote<SceneEditorNote>())
                    {
                        case SceneEditorNote.DocInitDone:
                            Domain domain = DomainManager.Instance().getDomain(this);
                            mEntityMng = domain.getCurrentDoc().EntityMng as SceneEntityManager;
                            mWorld = domain.getCurrentDoc().World as SceneWorld;
                            break;
                    }
                    break; ;
            }

            return base.notified(from, msg);
        }

        //Engine.PrimitiveGrid m_grid = null;
        SceneEntityManager mEntityMng = null;
        SceneWorld mWorld = null;
    }
}
