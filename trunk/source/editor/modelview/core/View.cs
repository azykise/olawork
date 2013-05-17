using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using editor;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;
using System.Windows.Forms;

namespace modelview
{
    class MainView : Common3DView
    {
        public MainView(Engine.RenderSystem rs,ModelViewDoc doc) :
            base("ModelMainView",rs)
        {
            m_camera.Position = new Vector3F(0, -5, 3);
            m_camera.Direction = new Vector3F(0, 0, 2);
            m_camera.Focus = true;
            Camera3DController camera_contrlooer = new Camera3DController("cam");
            camera_contrlooer.MoveSpeed = 0.2f;
            camera_contrlooer.WheelSpeed = 0.1f;
            BaseController = camera_contrlooer;

            mLightCtrl = new LightingController(doc);

            BaseController.OnKeyPressing += _onViewKeyPressing;
            BaseController.OnKeyUp += _onViewKeyUp;

            mDoc = doc;
        }

        public override void render()
        {
            float elasped = Tool.GetElaspedTime(ref mLastTick);

            m_camera.doUpdate(m_render_sys);                                      

            m_window.setBackgroundColor(0, 0, 1, 0);
            m_render_sys.setRenderWindow(m_window);           

            if (mDoc.CurrentStaticModel != null)
            {
                mDoc.CurrentStaticModel.render();
            }

            mDoc.ActionMng.update(elasped);

            if (mDoc.CurrentDynamicModel != null)
            {
                mDoc.CurrentDynamicModel.render();
            }

            if(mDoc.Grid != null)
                mDoc.Grid.render(m_render_sys);

            m_render_sys.beginDraw();
            m_render_sys.drawSceneActors();
            m_render_sys.endDraw();
        }

        protected HandleResult _onViewKeyPressing(KeyEventArgs key)
        {
            switch (key.KeyCode)
            {
                case Keys.L:
                    BaseController.CurrentSubController = mLightCtrl;
                    break;
            }
            return HandleResult.Done;
        }

        protected HandleResult _onViewKeyUp(KeyEventArgs key)
        {
            switch (key.KeyCode)
            {
                case Keys.L:
                    BaseController.CurrentSubController = null;
                    break;
            }
            return HandleResult.Done;
        }

        LightingController mLightCtrl = null;
        ModelViewDoc mDoc = null;
        long mLastTick = 0;
    }
}
