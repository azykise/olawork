using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace editor
{
    public partial class EditorLog_Form : Form
    {
        public const string LOG_FILE = "editor.log";
        public EditorLog_Form()
        {
            InitializeComponent();
        }

        bool mFileCreated = false;

        public void addNewLog(string log)
        {
            m_log.Add(log + Environment.NewLine);
            this.textBox_log.AppendText(log + Environment.NewLine);
            this.textBox_log.Refresh();

            FileStream fs = null;
            if (!File.Exists(LOG_FILE) || !mFileCreated)
            {
                mFileCreated = true;
                fs = File.Open(LOG_FILE, FileMode.Create);
            }
            else
                fs = File.Open(LOG_FILE, FileMode.Append);
            log += Environment.NewLine;
            Byte[] info = new UTF8Encoding(true).GetBytes(log);
            fs.Write(info, 0, info.Length);

            fs.Close();     
        }

        private void EditorLog_Form_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                this.Hide();
                e.Cancel = true;
            }
        }

        protected void onDiposing()
        {
             
                               
        }

        List<string> m_log = new List<string>();
    }

    class EditorLog : EditorComponent
    {
        public EditorLog() :
            base("Log")
        {
            m_log_form = new EditorLog_Form();
            m_log_form.Hide();
        }

        public EditorLog_Form LogForm
        {
            get { return m_log_form; }
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            if (msg.MsgType != MessageType.Log || msg.Messages.Count < 1)
                return NotifyResult.Pending;

            if(msg.Messages.Count == 1)
                m_log_form.addNewLog(msg.Messages[0]);
            else if (msg.Messages.Count > 1)
            {
                string msgex = msg.Messages[0] + ": " +DateTime.Now.ToString() + ": " + msg.Messages[1];
                m_log_form.addNewLog(msgex);
            }
            
            return NotifyResult.Succeed;
        }

        EditorLog_Form m_log_form = null;
    }
}
