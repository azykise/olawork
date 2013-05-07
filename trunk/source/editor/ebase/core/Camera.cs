using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    public enum ProjectionType
    {
        Prespective,
        Top,
        Front,
        Left
    }

    public class Camera //摄像机不分3D和2D 3D摄像机可以处理2D任务
    {
        public Camera()
        {
        }

        public virtual Matrix4F getViewMatrix()
        {
            return m_viewmat;
        }

        public virtual Matrix4F getProjMatrix()
        {
            return m_projmat;
        }

        public virtual Vector3F Direction
        {
            get { return m_direction; }
            set { m_direction = value; setViewParam();}
        }

        public virtual Vector3F Position
        {
            get { return m_position; }
            set { m_position = value; setViewParam(); }
        }

        public virtual ProjectionType ProjType
        {
            get { return m_proj_type; }
            set { m_proj_type = value; }
        }

        public virtual float OrthoW
        {
            get { return m_ortho_w; }
            set { m_ortho_w = value; }
        }

        public virtual float OrthoH
        {
            get { return m_ortho_h; }
            set { m_ortho_h = value; }
        }

        public virtual float Aspect
        {
            get { return m_aspect; }
            set { m_aspect = value; setViewParam(); }
        }

        public virtual bool Focus
        {
            get { return m_focus; }
            set { m_focus = value; }
        }

        public virtual void setViewParam()
        {
            m_view_param.Position[0] = m_position.X;
            m_view_param.Position[1] = m_position.Y;
            m_view_param.Position[2] = m_position.Z;

            m_view_param.LookAtPt[0] = m_direction.X;
            m_view_param.LookAtPt[1] = m_direction.Y;
            m_view_param.LookAtPt[2] = m_direction.Z;
            m_view_param.Aspect = m_aspect;
        }

        public virtual void doUpdate( RenderSystem rendersys)
        {
            rendersys.setViewParament(m_view_param);
            m_viewmat = (Matrix4F)rendersys.getViewMatrix();
            m_projmat = (Matrix4F)rendersys.getProjMatrix();
        }

        public virtual Vector3F getScreenWorldLocation(int x,int y,View view)
        {
            this.doUpdate(view.RenderSystem);

            int w = view.RenderWindow.getWidth();
            int h = view.RenderWindow.getHeight();

            Vector4F vec = new Vector4F();
            vec.X = m_view_param.NearDist * (((2.0f * x) / w) - 1.0f);
            vec.Y = m_view_param.NearDist * (((2.0f * (h - y)) / h) - 1.0f);
            vec.Z = 0.0f;
            vec.W = m_view_param.NearDist;

            Matrix4F vpInverse = (m_projmat * m_viewmat).Inverse();

            vec = vpInverse * vec;
            vec /= vec.W;

            return new Vector3F(vec.X, vec.Y, vec.Z);
        }

        protected bool m_focus = false;
        protected float m_aspect = 1.333f;
        protected float m_ortho_w = 1.0f;
        protected float m_ortho_h = 1.0f;
        protected ProjectionType m_proj_type = ProjectionType.Prespective;
        protected Vector3F m_position = new Vector3F(0, 0, 0);
        protected Vector3F m_direction = new Vector3F(0, 1, 0);
        protected Matrix4F m_viewmat = new Matrix4F();
        protected Matrix4F m_projmat = new Matrix4F();
        protected  ViewParament m_view_param = new  ViewParament();
    }
}
