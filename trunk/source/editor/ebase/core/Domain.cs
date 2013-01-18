using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public abstract class Domain : EditorComponent
    {
        public Domain(string name,string showtext)
            : base(name)
        {
            m_show_text = showtext;
        }

        public virtual void init(Engine.RenderSystem rsys)
        {
            m_view_mng.RenderSystem = rsys;
        }

        public virtual ViewManager getViewManager()
        {
            return m_view_mng;
        }

        public virtual EditorDoc getCurrentDoc()
        {
            return m_current_doc;
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Succeed;
        }

        public virtual void release()
        {
            if (!m_released)
            {
                if(m_current_doc != null)
                    m_current_doc.release();
                m_released = true;
            }            
        }

        public virtual string ShowText
        {
            get { return m_show_text; }
            set { m_show_text = value; }
        }

        public virtual System.Windows.Forms.Control MainForm { get { return m_main_form; } }        
        public virtual bool Active
        {
            get { return m_active; }
            set 
            { 
                m_active = value;
                m_view_mng.setViewActive(m_active);                    
            }
        }

        protected EditorDoc m_current_doc = null;
        protected ViewManager m_view_mng = new ViewManager();
        protected System.Windows.Forms.Control m_main_form = null;
        protected bool m_active = false;
        private bool m_released = false;
        protected string m_show_text = "undefined_domain";
    }
}
