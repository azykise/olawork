using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor
{
    public partial class Editor_Form : Form 
    {
        public Editor_Form()
        {
            InitializeComponent();
            EditorGraph.init();
            DomainManager.Instance().setBaseForm(this);
            m_domain_mng = DomainManager.Instance();
            EditorGraph.registerComponent(null, m_domain_mng);

            mComponent = new MainFormComponent(this);
            EditorGraph.registerComponent(null, mComponent);

            m_log = new EditorLog();
            EditorGraph.registerComponent(null, m_log);
            this.AddOwnedForm(m_log.LogForm);            
        }

        MainFormComponent mComponent = null;

        public bool initEditor(string[] configfile)
        {
            if (!m_enable)
            {               
                try 
                { 
                    m_render_sys = new RenderSystem(this.Handle); 
                }
                catch (System.Exception e) 
                { 
                    CommonTool.newLog(m_domain_mng, "RenderSystem 创建失败" + e.Message);
                }

                CommonTool.newLog(m_domain_mng, "RenderSystem 创建成功");

                if (!m_domain_mng.init("", m_render_sys))
                {
                    CommonTool.newLog(m_domain_mng, "Domain 创建失败");
                    this.Close();
                }
                else
                {
                    CommonTool.newLog(m_domain_mng, "Domain 创建成功");
                }

                setDomainButtom(m_domain_mng.Domains);

                if (m_domain_mng.MainDomain != null)
                {
                    setMainForm(m_domain_mng.MainDomain.MainForm);
                }              

                m_enable = true;
            }
            return m_enable;
        }

        public void setDomainButtom(List<Domain> domains)
        {            
            foreach (Domain domain in domains)
            {
                System.Windows.Forms.ToolStripMenuItem button_item = new System.Windows.Forms.ToolStripMenuItem();
                button_item.Text = domain.ShowText;
                s_buttomdomains[button_item] = domain;
                button_item.Click += delegate(object sender, EventArgs e)
                {
                    System.Windows.Forms.ToolStripMenuItem _button_item = sender as System.Windows.Forms.ToolStripMenuItem;
                    if (_button_item != null && s_buttomdomains.ContainsKey(_button_item))
                    {
                        Domain _domain = s_buttomdomains[_button_item];
                        if (m_domain_mng.MainDomain != null)
                        {
                            if (m_domain_mng.MainDomain.Equals(_domain))
                                return;
                            else
                                m_domain_mng.MainDomain.Active = false;
                        }

                        m_domain_mng.MainDomain = _domain;
                        m_domain_mng.MainDomain.Active = true;

                        setMainForm(m_domain_mng.MainDomain.MainForm);
                    }
                };
                this.buttonItem_domain.DropDownItems.Add(button_item);
            }
        }

        private void setMainForm(Control c)
        {
            if (c == null)
                return;
            this.panelBack.Controls.Clear();
            this.panelBack.Controls.Add(c);
            c.Dock = DockStyle.Fill;
            c.Show();
        }

        private void release(object sender, System.EventArgs e)
        {
            EditorGraph.release();
            m_domain_mng.release();
            m_render_sys.release();
        }

        //int frame_counter = 0;
        private void m_timer_Tick(object sender, System.EventArgs e)
        {
            m_timer.Stop();
            m_domain_mng.loop();
            //System.Console.WriteLine("frame count: " + (frame_counter++).ToString());
            m_timer.Start();
        }

        void Form1_Resize(object sender, System.EventArgs e)
        {

        }

        private void Editor_Form_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        private void buttonItem_log_Click(object sender, EventArgs e)
        {
            m_log.LogForm.Visible = !m_log.LogForm.Visible;
        }

        public DomainManager DomainMng { get { return m_domain_mng; } }

        DomainManager m_domain_mng = null;
        RenderSystem m_render_sys = null;
        EditorLog m_log = null;
        bool m_enable = false;

        static Dictionary<System.Windows.Forms.ToolStripMenuItem, Domain> s_buttomdomains = new Dictionary<System.Windows.Forms.ToolStripMenuItem, Domain>();
    }
}
