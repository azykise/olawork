using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor.ui
{    
    public enum Control3DStateChangeNote : int
    {
        Control3DSelectDrag = 457,
        Control3DMove,
        Control3DScale,
        Control3DRotate,
        Control3DFocus,
        Control3DGridOn,
        Control3DGridOff,
    }

    public partial class ToolBar3DControl : UserControl
    {
        public enum ControlState : int
        {
            SelectDrag = 100,
            Move,
            Scale,
            Rotate,
        }

        public ToolBar3DControl(string workarea)
        {
            InitializeComponent();
            m_workarea = workarea;
            m_editcomp = new ToolBar3DComponent(this);
        }

        public ControlState State
        {
            get { return m_state; }
            set { m_state = value; }
        }

        ControlState m_state = ControlState.SelectDrag;
        bool m_gridon = true;
        string m_workarea = "";
        public EditorComponent EditorComponent { get { return m_editcomp; } }
        public System.Windows.Forms.ToolStrip ToolBar { get { return toolStrip1; } }

        
        private void refreshState()
        {
            switch (m_state)
            {
                case ControlState.SelectDrag:
                    this.buttonSelect.Checked = true;
                    this.buttonScale.Checked = false;
                    this.buttonMove.Checked = false;
                    this.buttonRotate.Checked = false;
                    break;
                case ControlState.Move:
                    this.buttonSelect.Checked = false ;
                    this.buttonScale.Checked = false;
                    this.buttonMove.Checked = true;
                    this.buttonRotate.Checked = false;
                    break;
                case ControlState.Scale:
                    this.buttonSelect.Checked = false;
                    this.buttonScale.Checked = true;
                    this.buttonMove.Checked = false;
                    this.buttonRotate.Checked = false;
                    break;
                case ControlState.Rotate:
                    this.buttonSelect.Checked = false;
                    this.buttonScale.Checked = false;
                    this.buttonMove.Checked = false;
                    this.buttonRotate.Checked = true;
                    break; 
            }
        }

        private void buttonSelect_Click(object sender, EventArgs e)
        {
            m_state = ControlState.SelectDrag;
            refreshState();
            EditorGraph.broadCast(m_editcomp, new MessageBase(MessageType.Editor, Control3DStateChangeNote.Control3DScale));
        }

        private void buttonMove_Click(object sender, EventArgs e)
        {
            m_state = ControlState.Move;
            refreshState();
            EditorGraph.broadCast(m_editcomp, new MessageBase(MessageType.Editor, Control3DStateChangeNote.Control3DMove));
        }

        private void buttonRotate_Click(object sender, EventArgs e)
        {
            m_state = ControlState.Rotate;
            refreshState();
            EditorGraph.broadCast(m_editcomp, new MessageBase(MessageType.Editor, Control3DStateChangeNote.Control3DRotate));
        }

        private void buttonScale_Click(object sender, EventArgs e)
        {
            m_state = ControlState.Scale;
            refreshState();
            EditorGraph.broadCast(m_editcomp, new MessageBase(MessageType.Editor, Control3DStateChangeNote.Control3DScale));
        }

        private void buttonGrid_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.ToolStripButton buttom = sender as System.Windows.Forms.ToolStripButton;
            if (buttom == null)
                return;
            m_gridon = !m_gridon;
            buttom.Checked = m_gridon;
            EditorGraph.broadCast(m_editcomp, new MessageBase(MessageType.Editor, m_gridon ? Control3DStateChangeNote.Control3DGridOn : Control3DStateChangeNote.Control3DGridOff));
        }
        

        public class ToolBar3DComponent : EditorComponent
        {
            public ToolBar3DComponent(ToolBar3DControl bar) :
                base(bar.m_workarea + "_ToolBar3DComponent")
            {
                m_bar = bar;
            }

            public override NotifyResult notified(EditorComponent from, MessageBase msg)
            {
                return NotifyResult.Pending;
            }

            ToolBar3DControl m_bar = null;
        }
        ToolBar3DComponent m_editcomp = null;
    }
}
