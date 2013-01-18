using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public abstract class View : EditorComponent
    {
        public View(string name,Engine.RenderSystem rs)
            : base(name)
        {
            m_render_sys = rs;
        }

        public virtual void setViewMng(ViewManager mng)
        {
            m_mng = mng;
        }

        public virtual void update(float elapsed)
        {

        }

        public virtual void render()
        {

        }

        public virtual Camera getCamera() { return m_camera; }

        public virtual HandleResult handleMouseEvent(MouseInputEvent e)
        {
            if (m_base_controller != null)
                return m_base_controller.handleMouseEvent(e);
            else
                return HandleResult.Pending;
        }

        public virtual HandleResult handleKeyboardEvent(KeyboardInputEvent e)
        {
            HandleResult r1 = HandleResult.Pending;
            HandleResult r2 = HandleResult.Pending;

            if (m_base_controller != null)
                r1 = m_base_controller.handleKeyboardEvent(e);

            r2 = m_keyboard_listener.handleEvent(e);

            return r1 == HandleResult.Done || r2 == HandleResult.Done ? HandleResult.Done : HandleResult.Pending;
        }

        public virtual void resizeWindow(int new_w,int new_h,IntPtr hwnd)
        {
            if (m_window != null)
                m_window.resetWindow(new_w, new_h, hwnd);
            m_camera.Aspect = (float)new_w / (float)new_h;
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Succeed;
        }

        public virtual Engine.RenderSystem RenderSystem{get{return m_render_sys;}}
        public virtual Engine.RenderWindow RenderWindow
        {
            get{return m_window;}
            set{m_window = value;}
        }
        public virtual bool Active
        {
            get { return m_active; }
            set { m_active = value; }
        }
        public virtual ComplexController BaseController
        {
            get { return m_base_controller; }
            set 
            { 
                m_base_controller = value;
                m_base_controller.ControlView = this;
            }
        }
        public virtual ViewKeyboardListener KeyBoardListner
        {
            get { return m_keyboard_listener; }
            set { m_keyboard_listener = value; }
        }

        protected Camera m_camera = new Camera();
        protected Engine.RenderSystem m_render_sys = null;
        protected Engine.RenderWindow m_window = null;
        protected ViewManager m_mng;

        protected ComplexController m_base_controller = null;
        protected ViewKeyboardListener m_keyboard_listener = new ViewKeyboardListener();
        protected bool m_active = false;
    }

    public class ViewManager
    {
        public ViewManager()
        {

        }

        public virtual void addView(View v)
        {
            if (!m_views.Contains(v))
            {
                m_views.Add(v);
                v.setViewMng(this);
            }
        }

        public virtual void setViewActive(bool a)
        {
            foreach (View v in m_views)
            {
                v.Active = a;
            }
        }

        public virtual void update(float elasped)
        {
            foreach (View v in m_views)
            {
                v.update(elasped);
            }
        }

        public virtual void render()
        {
            foreach (View v in m_views)
            {
                if (!v.Active)
                    continue;

                m_render_system.setRenderWindow(v.RenderWindow);
                v.render();
            }
        }

        public virtual View CurrentView
        {
            get { return m_current_view; }
            set { m_current_view = value; }
        }
        public virtual List<View> Views
        {
            get { return m_views; }
        }
        public virtual Engine.RenderSystem RenderSystem
        {
            get { return m_render_system; }
            set { m_render_system = value; }
        }

        protected Engine.RenderSystem m_render_system = null;
        protected View m_current_view = null;
        protected List<View> m_views = new List<View>();
    }
}
