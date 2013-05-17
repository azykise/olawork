using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    public enum HandleResult
    {
        Done,       //已处理,不需要下层控制器处理
        Pending,    //未处理,交给下层控制器处理
        Leave,      //控制器离开
    }

    public abstract class Controller : EditorComponent
    {
        public Controller(string name)
            : base(name)
        {
        }

        //鼠标事件按下列顺序发生:MSDN
        //MouseEnter
        //MouseMove
        //MouseHover / MouseDown / MouseWheel
        //MouseUp
        //MouseLeave
        public virtual HandleResult handleMouseEvent(MouseInputEvent e)
        {
            if (m_current_view == null)
                return HandleResult.Pending;

            HandleResult result = HandleResult.Pending;

            MouseButtons button = e.Args.Button;
            Point locatoin = e.Args.Location;
            InputAction action = e.Action;

            
            if (action == InputAction.OnMouseKeyDown) 
            {
                if(button == MouseButtons.Left)
                {
                    m_left_down_staying = true;
                    m_left_down = true;
                    result = OnMouseLeftDown != null ? OnMouseLeftDown(locatoin) : result;
                }
                else if(button == MouseButtons.Right)
                {
                    m_right_down_staying = true;
                    m_right_down = true;
                    result = OnMouseRightDown != null ? OnMouseRightDown(locatoin) : result;
                }
                else if(button == MouseButtons.Middle)
                {
                    result = OnMouseWheelDown != null ? OnMouseWheelDown(locatoin) : result;
                }
                m_last_down_location = locatoin;
            }
            else if (action == InputAction.OnMouseKeyUp)
            {
                if (button == MouseButtons.Left)
                {
                    m_left_down = false;
                    if (m_left_down_staying)
                        result = this.OnMouseLeftStayingUp != null ? OnMouseLeftStayingUp(locatoin) : result;
                    else
                        result = this.OnMouseLefttDragingUp != null ? OnMouseLefttDragingUp(locatoin) : result; 

                    result = OnMouseLeftUp != null ? OnMouseLeftUp(locatoin) :result ;
                }
                else if (button == MouseButtons.Right)
                {
                    m_right_down = false;
                    if (m_right_down_staying)
                        result = this.OnMouseRightStayingUp != null ? OnMouseRightStayingUp(locatoin) : result;
                    else
                        result = this.OnMouseRightDragingUp != null ? OnMouseRightDragingUp(locatoin) : result;
                    
                    result = OnMouseRightUp != null ? OnMouseRightUp(locatoin) : result;
                }
                else if (button == MouseButtons.Middle)
                {
                    result = OnMouseWheelUp != null ? OnMouseWheelUp(locatoin) : result;
                }
            }
            else if (action == InputAction.OnMouseDraging)
            {                
                Point delta = new Point(locatoin.X - m_last_mouse_location.X, locatoin.Y - m_last_mouse_location.Y);
                if (button == MouseButtons.Left )
                {
                    m_left_down_staying = false;
                    result = OnLeftDraging != null ? OnLeftDraging(locatoin, delta) : result;
                }
                else if (button == MouseButtons.Right)
                {
                    m_right_down_staying = false;
                    result = OnRightDraging != null ? OnRightDraging(locatoin, delta) : result;
                }
                else if (button == MouseButtons.Middle )
                {
                    result = OnWheelDraging != null ? OnWheelDraging(locatoin, delta) : result;
                }
            }
            else if (action == InputAction.OnWheel )
            {
                if (m_left_down && OnMouseLeftDownWheeling != null)
                {
                    m_left_down_staying = false;
                    result = OnMouseLeftDownWheeling(locatoin, e.Args.Delta);
                }
                else if (m_right_down && OnMouseRightDownWheeling != null)
                {
                    m_right_down_staying = false;
                    result = OnMouseRightDownWheeling(locatoin, e.Args.Delta);
                }
                else
                    result = OnMouseWheeling != null ? OnMouseWheeling(locatoin, e.Args.Delta) : result;
            }
            else if (action == InputAction.OnMouseMoving)
            {
                m_right_down_staying = false;
                Point delta = new Point(locatoin.X - m_last_mouse_location.X, locatoin.Y - m_last_mouse_location.Y);
                result = OnMouseMoving != null ? OnMouseMoving(locatoin, delta) : result;
            }

            m_last_mouse_location = locatoin;

            return result;
        }

        public virtual HandleResult handleKeyboardEvent(KeyboardInputEvent e)
        {
            InputAction action = e.Action;

            if (action == InputAction.OnKeyDown && OnKeyDown != null)
                OnKeyDown(e.Args);
            else if (action == InputAction.OnKeyPressing && OnKeyPressing != null)
                OnKeyPressing(e.Args);
            else if (action == InputAction.OnKeyUp && OnKeyUp != null)
                OnKeyUp(e.Args);

            return HandleResult.Pending;
        }

        public virtual void render()
        {

        }

        public virtual void update(float elapsed)
        {

        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Succeed;
        }

        public virtual View ControlView 
        { 
            get { return m_current_view; }
            set { m_current_view = value; }
        }

        private bool m_left_down_staying = false;
        private bool m_right_down_staying = false; //鼠标右键按下到鼠标右键抬起之间鼠标是否移动过
        private bool m_left_down = false;
        private bool m_right_down = false;
        protected View m_current_view = null;
        protected Point m_last_mouse_location = new Point(0, 0);
        protected Point m_last_down_location = new Point(0, 0);

        public delegate HandleResult MouseButtonEvent(Point location);
        public delegate HandleResult MouseWheelEvent(Point location, int delta);
        public delegate HandleResult MouseMoveEvent(Point location, Point delta);

        public delegate HandleResult KeyboardEvent(KeyEventArgs key);

        public event MouseButtonEvent OnMouseLeftDown = null;
        public event MouseButtonEvent OnMouseLeftUp = null;
        public event MouseButtonEvent OnMouseLeftStayingUp = null;
        public event MouseButtonEvent OnMouseLefttDragingUp = null;

        public event MouseButtonEvent OnMouseRightDown = null;
        public event MouseButtonEvent OnMouseRightUp = null;
        public event MouseButtonEvent OnMouseRightStayingUp = null;
        public event MouseButtonEvent OnMouseRightDragingUp = null;

        public event MouseButtonEvent OnMouseWheelDown = null;
        public event MouseButtonEvent OnMouseWheelUp = null;
        public event MouseWheelEvent OnMouseLeftDownWheeling = null;
        public event MouseWheelEvent OnMouseRightDownWheeling = null;
        public event MouseWheelEvent OnMouseWheeling = null;

        public event MouseMoveEvent OnMouseMoving = null;
        public event MouseMoveEvent OnLeftDraging = null;
        public event MouseMoveEvent OnRightDraging = null;
        public event MouseMoveEvent OnWheelDraging = null;

        public event KeyboardEvent OnKeyDown = null;
        public event KeyboardEvent OnKeyPressing = null;
        public event KeyboardEvent OnKeyUp = null;
    }

    //复合鼠标控制器
    public abstract class ComplexController : Controller
    {
        public ComplexController(string name)
            : base(name)
        {

        }

        //复合鼠标控制器用来建构一个消息处理链
        //消息在消息链中被各个控制器处理
        public override HandleResult handleMouseEvent(MouseInputEvent e)
        {
            HandleResult result = HandleResult.Pending;

            if (m_current_sub != null)//当前和默认子控制器都可以为null,即没有消息链
            {
                result = m_current_sub.handleMouseEvent(e); //先使用当前子控制器处理消息
                if (result == HandleResult.Leave)
                    m_current_sub = m_default_sub; //当前子控制器退出之后设置为默认子控制器
                else if (result == HandleResult.Done)
                    return result;
            }

            result = base.handleMouseEvent(e);

            return result;
        }

        public override HandleResult handleKeyboardEvent(KeyboardInputEvent e)
        {
            HandleResult result = HandleResult.Pending;

            if (m_current_sub != null)
            {
                result = m_current_sub.handleKeyboardEvent(e); 
                if (result == HandleResult.Leave)
                    m_current_sub = m_default_sub; 
                else if (result == HandleResult.Done)
                    return result;
            }

            result = base.handleKeyboardEvent(e);;

            return result;
        }

        public virtual Controller DefaultSubController
        {
            get { return m_default_sub; }
            set 
            { 
                m_default_sub = value;
                m_default_sub.ControlView = m_current_view;
            }
        }
        public virtual Controller CurrentSubController
        {
            get { return m_current_sub; }
            set 
            {
                m_current_sub = value;
                if(m_current_sub != null)
                    m_current_sub.ControlView = m_current_view;
            }
        }

        public override void render()
        {
            base.render();
            if (m_current_sub != null)
                m_current_sub.render();
        }

        //复合鼠标控制器具备默认子控制器和当前子控制器
        //子控制器也可以是复合控制器
        protected Controller m_default_sub = null;
        protected Controller m_current_sub = null;
    }

    public class Camera3DController : ComplexController
    {
        public Camera3DController(string name):
            base(name)
        {
            this.OnRightDraging += new MouseMoveEvent(Camera3DController_OnRightDraging);
            this.OnKeyPressing += new KeyboardEvent(Camera3DController_OnKeyPressing);
            this.OnKeyDown += new KeyboardEvent(Camera3DController_OnKeyDown);
            this.OnMouseWheeling += new MouseWheelEvent(Camera3DController_OnMouseWheeling);
            this.OnWheelDraging += new MouseMoveEvent(Camera3DController_OnWheelDraging);
            //this.OnMouseRightDragingUp += new MouseButtonEvent(Camera3DController_OnMouseRightDragingUp);
            //this.OnMouseRightStayingUp += new MouseButtonEvent(Camera3DController_OnMouseRightStayingUp);
            this.OnMouseRightDownWheeling += new MouseWheelEvent(Camera3DController_OnMouseRightDownWheeling);
        }

        HandleResult Camera3DController_OnMouseRightDownWheeling(Point location, int delta)
        {
            Vector3F lookatpt = m_current_view.getCamera().Direction;
            Vector3F postiton = m_current_view.getCamera().Position;

            float df = 0.0f;
            if (delta > 0)
                df = 1.0f;
            else if (delta < 0)
                df = -1.0f;

            Vector3F dv = new Vector3F(0, 0, df * m_wheel_speed * -1.0f);
            m_current_view.getCamera().Position = postiton + dv;
            m_current_view.getCamera().Direction = lookatpt + dv;
            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnWheelDraging(Point location, Point delta)
        {
            if (Focus || m_current_view.getCamera().Focus)
            {
                return HandleResult.Done;
            }

            Vector3F lookatpt = m_current_view.getCamera().Direction;
            Vector3F postiton = m_current_view.getCamera().Position;

            Vector3F dir = lookatpt - postiton;
            Vector3F top = Engine.Math.IsZero(dir.X) && Engine.Math.IsZero(dir.Y) ? Vector3F.YAxis : Vector3F.ZAxis;

            Vector3F right = Vector3F.CrossProduct(dir, top);
            right.Normalize();
            Vector3F up = Vector3F.CrossProduct(right, dir);
            up.Normalize();

            Vector3F front = Vector3F.CrossProduct(Vector3F.ZAxis, right);
            front.Normalize();

            postiton = postiton + front * delta.Y * m_move_speed;
            lookatpt = lookatpt + front * delta.Y * m_move_speed;

            postiton = postiton + right * delta.X * -1.0f * m_move_speed;
            lookatpt = lookatpt + right * delta.X * -1.0f * m_move_speed;
            m_current_view.getCamera().Position = postiton;
            m_current_view.getCamera().Direction = lookatpt;

            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnMouseRightStayingUp(Point location)
        {
            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnMouseRightDragingUp(Point location)
        {
            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnMouseWheeling(Point location, int delta)
        {
            Vector3F lookatpt = m_current_view.getCamera().Direction;
            Vector3F postiton = m_current_view.getCamera().Position;

            Vector3F dist = lookatpt - postiton;
            float length = dist.GetLength();
            dist.Normalize();

            if (delta > 0 && length <= 1.0f)
                return HandleResult.Done;

            float df = 0.0f;
            if (delta > 0)
                df = 1.0f;
            else if (delta < 0)
                df = -1.0f;

            m_current_view.getCamera().Position = postiton + dist * m_wheel_speed * df;
            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnRightDraging(Point location, Point delta)
        {
            Vector3F lookatpt = m_current_view.getCamera().Direction;
            Vector3F postiton = m_current_view.getCamera().Position;

            Vector3F dir = lookatpt - postiton;      
            Vector3F top = Engine.Math.IsZero(dir.X) && Engine.Math.IsZero(dir.Y) ? Vector3F.YAxis : Vector3F.ZAxis;

            Vector3F right = Vector3F.CrossProduct(dir, top);
            right.Normalize();
            Vector3F up = Vector3F.CrossProduct(right, dir);
            up.Normalize();

            QuaternionF q1 = QuaternionF.FromAxisAngle(right, delta.Y * -1.0f * m_rotate_speed);
            QuaternionF q2 = QuaternionF.FromAxisAngle(up, delta.X * -1.0f * m_rotate_speed);
            q1.Normalize();
            q2.Normalize();

            QuaternionF q = q1 * q2; q.Normalize();
            Matrix4F m = QuaternionF.QuaternionToMatrix(q);
            Vector4F v_rotate = new Vector4F(dir.X, dir.Y, dir.Z, 0);
            v_rotate = m * v_rotate;
            dir = new Vector3F(v_rotate.X, v_rotate.Y, v_rotate.Z);

            if (Focus || m_current_view.getCamera().Focus)
                m_current_view.getCamera().Position = lookatpt - dir;
            else
                m_current_view.getCamera().Direction = postiton + dir;

            return HandleResult.Done;
        }

        HandleResult Camera3DController_OnKeyDown(KeyEventArgs key)
        {
            return Camera3DController_OnKeyPressing(key);
        }

        HandleResult Camera3DController_OnKeyPressing(KeyEventArgs key)
        {
            Vector3F lookatpt = m_current_view.getCamera().Direction;
            Vector3F postiton = m_current_view.getCamera().Position;

            Vector3F dir = lookatpt - postiton;
            dir.Normalize();

            Vector3F right = Vector3F.CrossProduct(dir, new Vector3F(0, 0, 1));
            right.Normalize();
            Vector3F up = Vector3F.CrossProduct(right, dir);
            up.Normalize();

            if (Focus || m_current_view.getCamera().Focus)
            {
                return HandleResult.Done;
            }

            switch (key.KeyCode)
            {
                case Keys.W:
                    m_current_view.getCamera().Position = postiton + dir * m_move_speed;
                    m_current_view.getCamera().Direction = lookatpt + dir * m_move_speed;
                    break;
                case Keys.A:
                    m_current_view.getCamera().Position = postiton + right * m_move_speed;
                    m_current_view.getCamera().Direction = lookatpt + right * m_move_speed;
                    break;
                case Keys.D:
                    m_current_view.getCamera().Position = postiton - right * m_move_speed;
                    m_current_view.getCamera().Direction = lookatpt - right * m_move_speed;
                    break;
                case Keys.S:
                    m_current_view.getCamera().Position = postiton - dir * m_move_speed;
                    m_current_view.getCamera().Direction = lookatpt - dir * m_move_speed;
                    break;                
                default:
                    return HandleResult.Pending;
            }

            return HandleResult.Done;
        }

        public virtual bool Focus
        {
            get { return m_focus; }
            set { m_focus = true; }
        }

        public virtual float WheelSpeed 
        {
            get { return m_wheel_speed; }
            set { 
                m_wheel_speed = value;
            }
        }

        public virtual float MoveSpeed
        {
            get { return m_move_speed; }
            set { m_move_speed = value; }
        }

        protected float m_wheel_speed = 1.0f;
        protected float m_move_speed = 1.0f;
        protected float m_rotate_speed = 0.001f;
        protected bool m_focus = false;
    }
}
