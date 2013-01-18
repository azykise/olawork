using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Sharp3D.Math;

namespace editor
{
    public enum InputAction
    {
        None,
        OnKeyDown,
        OnKeyPressing,
        OnKeyUp,
        OnMouseKeyDown,
        OnMouseKeyUp,
        OnMouseDraging,
        OnMouseMoving,
        OnWheel,
    }

    public class MouseInputEvent
    {
        public MouseInputEvent(MouseEventArgs args, InputAction action)
        {
            m_action = action;
            m_args = args;
        }

        public virtual MouseEventArgs Args { get { return m_args; } }
        public virtual InputAction Action { get { return m_action; } }

        protected InputAction m_action;
        protected MouseEventArgs m_args = null;
    }

    public class KeyboardInputEvent
    {
        public KeyboardInputEvent(KeyEventArgs agrs, InputAction action)
        {
            m_args = agrs;
            m_action = action;
        }

        public virtual KeyEventArgs Args { get { return m_args; } }
        public virtual InputAction Action { get { return m_action; } }

        protected KeyEventArgs m_args = null;
        protected InputAction m_action;
    }

    public class ViewKeyboardListener
    {
        public ViewKeyboardListener()
        {

        }

        public delegate HandleResult KeyHandler(KeyboardInputEvent e);

        public virtual void setHandler(int k,KeyHandler h)
        {
            if (m_handlers.ContainsKey(k))
                m_handlers[k] += h;
            else
                m_handlers.Add(k, h);
        }

        public virtual HandleResult handleEvent(KeyboardInputEvent e)
        {
            int k = (int)(e.Args.KeyData);
            if(m_handlers.ContainsKey(k))
                return m_handlers[k](e);
            else
                return HandleResult.Pending;
        }

        protected Dictionary<int, KeyHandler> m_handlers = new Dictionary<int, KeyHandler>();
    }
}
