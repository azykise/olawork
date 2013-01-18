using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor
{
    // hardware render panel 硬件绘图 engine的frontbuffer就画在这上面
    public partial class HWRenderPanel : UserControl
    {
        public HWRenderPanel()
        {
            InitializeComponent();
        }

        private void HWRenderPanel_KeyDown(object sender, KeyEventArgs e)
        {
            if (m_key_pressed != e.KeyData)
            {
                m_key_pressed = e.KeyData;
                if (m_view != null)                
                    m_view.handleKeyboardEvent(new KeyboardInputEvent(e, InputAction.OnKeyDown));                   
                
            }
            else
            {
                if (m_view != null)
                    m_view.handleKeyboardEvent(new KeyboardInputEvent(e, InputAction.OnKeyPressing));
            }
        }

        private void HWRenderPanel_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void HWRenderPanel_KeyUp(object sender, KeyEventArgs e)
        {
            m_key_pressed = Keys.None;
        }

        private void HWRenderPanel_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {

        }

        private void HWRenderPanel_MouseWheel(object sender, MouseEventArgs e)
        {
            if (m_view != null)
                m_view.handleMouseEvent(new MouseInputEvent(e, InputAction.OnWheel));
        }

        private void HWRenderPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if(m_mouse_pressed != e.Button)
            {
                m_mouse_pressed = e.Button;
                if (m_view != null)
                {
                    MouseEventArgs args = new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
                    m_view.handleMouseEvent(new MouseInputEvent(args, InputAction.OnMouseKeyDown));
                }
            }           
        }

        private void HWRenderPanel_MouseUp(object sender, MouseEventArgs e)
        {
            m_mouse_pressed = MouseButtons.None;
            if (m_view != null)
            {
                MouseEventArgs args = new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
                m_view.handleMouseEvent(new MouseInputEvent(args, InputAction.OnMouseKeyUp));
            }            
        }

        private void HWRenderPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if(m_mouse_pressed == e.Button && e.Button != MouseButtons.None)
            {
                if (m_view != null)
                {
                    MouseEventArgs args = new MouseEventArgs(e.Button, e.Clicks, e.X, e.Y, e.Delta);
                    m_view.handleMouseEvent(new MouseInputEvent(args, InputAction.OnMouseDraging));
                }
            }
            else
            {
                if (m_view != null)
                {
                    MouseEventArgs args = new MouseEventArgs(e.Button, e.Clicks, e.X,e.Y, e.Delta);
                    m_view.handleMouseEvent(new MouseInputEvent(args, InputAction.OnMouseMoving));
                }
            }
        }

        private void HWRenderPanel_MouseEnter(object sender, EventArgs e)
        {
            if (!this.Focused)
                this.Focus();
        }

        private void HWRenderPanel_MouseHover(object sender, EventArgs e)
        {

        }

        private void HWRenderPanel_MouseLeave(object sender, EventArgs e)
        {
            
        }

        private void HWRenderPanel_Resize(object sender, EventArgs e)
        {
            if (m_view != null)
                m_view.resizeWindow(this.Width, this.Height, this.Handle);
        }

        public virtual View getCurrentView() { return m_view; }
        public virtual void setCurrentView(View v) 
        {
            m_view = v;
            m_view.Active = true;
            m_view.resizeWindow(this.Width, this.Height, this.Handle);
        }

        View m_view = null;
        MouseButtons m_mouse_pressed = MouseButtons.None;
        Keys m_key_pressed = Keys.None;
    }
}
