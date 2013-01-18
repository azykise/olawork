using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using System.Drawing;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    public class EntityCreateController : Controller
    {
        public EntityCreateController() :
            base("物品创建控制器")
        {
            initialize();
        }

        public EntityCreateController(string name):
            base(name)
        {
            initialize();
        }

        private void initialize()
        {
            this.OnMouseLeftDown += new MouseButtonEvent(EntityCreateController_OnMouseLeftDown);
            this.OnMouseRightStayingUp += new MouseButtonEvent(EntityCreateController_OnMouseRightStayingUp);
            this.OnMouseMoving += new MouseMoveEvent(EntityCreateController_OnMouseMoving);
        }

        HandleResult EntityCreateController_OnMouseMoving(System.Drawing.Point location, System.Drawing.Point delta)
        {
            return HandleResult.Done;
        }

        HandleResult EntityCreateController_OnMouseRightStayingUp(System.Drawing.Point location)
        {
            return HandleResult.Leave;
        }

        HandleResult EntityCreateController_OnMouseLeftDown(System.Drawing.Point location)
        {
            return HandleResult.Done;
        }

        Entity mHoldEntity = null;
    }

    public class EntitySelectDragController : Controller
    {
        public EntitySelectDragController(string name):
            base(name)
        {
            initialize();
        }

        public EntitySelectDragController() :
            base("物品选择控制器")
        {
            initialize();
        } 

        private void initialize()
        {
            this.OnMouseLeftDown += new MouseButtonEvent(EntitySelectDelectController_OnMouseLeftDown);
            this.OnKeyDown += new KeyboardEvent(EntitySelectDelectController_OnKeyDown);
            this.OnLeftDraging += new MouseMoveEvent(EntitySelectDelectController_OnLeftDraging);
            this.OnMouseLeftUp += new MouseButtonEvent(EntitySelectDelectController_OnMouseLeftUp);
        }

        HandleResult EntitySelectDelectController_OnMouseLeftUp(System.Drawing.Point location)
        {
            if (mShowRect && m_current_view != null)
            {
                Selector.doDragSelect(mLeftDownPT, location, m_current_view);
            }

            mShowRect = false;
            mRectPT[0] = mRectPT[1] = mRectPT[2] = mRectPT[3] = Vector3F.Zero;
            return HandleResult.Pending;
        }

        HandleResult EntitySelectDelectController_OnLeftDraging(System.Drawing.Point location, System.Drawing.Point delta)
        {
            if (mHoldEntitys.Count == 0)
                mShowRect = computeCameraRect(mLeftDownPT, location, m_current_view);
            else
            {                
                Domain domain = DomainManager.Instance().getDomain(m_current_view);
                EditWorld world = domain.getCurrentDoc().World;

                if (world != null)
                {
                    IntersectionPair res1 = Tool.Intersect(mLastDragPT, world.SeaLevel, m_current_view);
                    IntersectionPair res2 = Tool.Intersect(location, world.SeaLevel, m_current_view);
                    if (res1.IntersectionOccurred &&
                        res2.IntersectionOccurred)
                    {
                        Vector3F p1 = res1.IntersectionPoint;
                        Vector3F p2 = res2.IntersectionPoint;
                        Vector3F v = p2 - p1;

                        foreach (Entity entity in mHoldEntitys)
                        {
                            entity.Position = entity.Position + v;
                        }                        
                    }
                }

                mShowRect = false;
                mLastDragPT = location;
            }
            return HandleResult.Done;
        }

        HandleResult EntitySelectDelectController_OnMouseLeftDown(System.Drawing.Point location)
        {
            mShowRect = false;
            mLeftDownPT.X = location.X;
            mLeftDownPT.Y = location.Y;

            mHoldEntitys.Clear();
            if (Selector.doDownSelect(mLeftDownPT, m_current_view))
            {
                Domain domain = DomainManager.Instance().getDomain(m_current_view);
                EntityManager mng = domain.getCurrentDoc().EntityMng;
                mHoldEntitys.AddRange(mng.quickGetSelectedEntitys());                
            }            

            mLastDragPT = mLeftDownPT;
            return HandleResult.Pending;
        }

        HandleResult EntitySelectDelectController_OnKeyDown(System.Windows.Forms.KeyEventArgs key)
        {
            return HandleResult.Pending;
        }

        public override void render()
        {
            base.render();

            if (mSelectRect == null && m_current_view != null)
            {
                mSelectRect = m_current_view.RenderSystem.createPrimitiveGrid();
                mSelectRect.setColor(1.0f, 1.0f, 1.0f, 1.0f);
            }

            if (mShowRect && m_current_view != null)
            {
                mSelectRect.setRect(mRectPT[0], mRectPT[1], mRectPT[2], mRectPT[3]);
                mSelectRect.render(m_current_view.RenderSystem);
            }
        }

        private bool computeCameraRect(Point p1,Point p2,View view)
        {
            Rectangle r = Tool.MakeScreenRect(p1, p2);

            if (r.Right - r.Left <= 0 || r.Bottom - r.Top <= 0)
            {
                mRectPT[0] = mRectPT[1] = mRectPT[2] = mRectPT[3] = Vector3F.Zero;
                return false;
            }

            mRectPT[0] = view.getCamera().getScreenWorldLocation(r.Left, r.Top, view);
            mRectPT[1] = view.getCamera().getScreenWorldLocation(r.Right, r.Top, view);
            mRectPT[2] = view.getCamera().getScreenWorldLocation(r.Right, r.Bottom, view);
            mRectPT[3] = view.getCamera().getScreenWorldLocation(r.Left, r.Bottom, view);

            for (int i = 0; i < 4; i++ )
            {
                Vector3F dir = mRectPT[i] - view.getCamera().Position;
                dir.Normalize();
                dir = dir * 0.01f;
                mRectPT[i] += dir;
            }

            return true;
        }

        List<Entity> mHoldEntitys = new List<Entity>();
        Point mLeftDownPT = new Point(0, 0);
        Point mLastDragPT = new Point(0, 0);
        bool mShowRect = false;
        Vector3F[] mRectPT = new Vector3F[4];
        
        Engine.PrimitiveGrid mSelectRect = null;
    }

    public class EntityTransform3DController : Controller
    {
        public enum TransformState : uint
        {
            TRANS_NONE = 0,
            TRANS_MOVE,
            TRANS_ROTATE,
            TRANS_SCALE
        }
        public const string CLASS_NAME = "EntityTransform3DController";
        public EntityTransform3DController():
            base(CLASS_NAME)
        {
            this.OnMouseMoving += new MouseMoveEvent(EntityTransform3DController_OnMouseMoving);
            this.OnMouseLeftDown += new MouseButtonEvent(EntityTransform3DController_OnMouseLeftDown);
            this.OnLeftDraging += new MouseMoveEvent(EntityTransform3DController_OnLeftDraging);

            mMover.ParentController = this;
            mRotater.ParentController = this;
            mScaler.ParentController = this;
        }

        public void setTransformState(TransformState state)
        {
            mState = state;
            switch (mState)
            {
                case TransformState.TRANS_NONE:
                    mCurrentTransformer = null; break;
                case TransformState.TRANS_MOVE:
                    mCurrentTransformer = mMover; break;
                case TransformState.TRANS_ROTATE:
                    mCurrentTransformer = mRotater; break;
                case TransformState.TRANS_SCALE:
                    mCurrentTransformer = mScaler; break;                    
            }
            if(mCurrentTransformer != null)
                mCurrentTransformer.ParentController = this;
        }

        HandleResult EntityTransform3DController_OnLeftDraging(Point location, Point delta)
        {
            throw new NotImplementedException();
        }

        HandleResult EntityTransform3DController_OnMouseLeftDown(Point location)
        {
            throw new NotImplementedException();
        }

        HandleResult EntityTransform3DController_OnMouseMoving(Point location, Point delta)
        {
            throw new NotImplementedException();
        }

        public override void render()
        {
            if (m_current_view != null)
                mCurrentTransformer.render();
        }

        EntityMove3D    mMover = new EntityMove3D();
        EntityRotate3D mRotater = new EntityRotate3D();
        EntityScale3D mScaler = new EntityScale3D();
        EntityTransform3D mCurrentTransformer = null;

        TransformState mState = TransformState.TRANS_NONE;
    }
}
