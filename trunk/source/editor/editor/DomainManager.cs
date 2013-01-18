using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Windows.Forms;
using System.Reflection;

namespace editor
{
    class DomainManager : EditorComponent
    {
        public DomainManager(Editor_Form base_form):
            base("DomainManager")
        {
            m_base_form = base_form;
        }

        public void loop()
        {
            foreach (Domain domain in m_domains)
            {
                if (domain.Active)
                {
                    domain.getViewManager().update(0.0f);
                }
            }

            foreach (Domain domain in m_domains)
            {
                if (domain.Active)
                {
                    domain.getViewManager().render();
                }
            }
        }

        public bool init(string info,Engine.RenderSystem render_sys)
        {
            XmlDocument xml = new XmlDocument();
            xml.Load(ConfigPath);

            XmlNode root = xml.SelectSingleNode("EditorConfig");
            if(root == null)
            {
                MessageBox.Show("无法读取配置文件");
                return false;
            }
            XmlNode plugin_node = root.SelectSingleNode("Plugin");
            if(plugin_node != null)
            {
                string plugin_path = plugin_node.Attributes["path"].Value;
                string plugin_main = plugin_node.Attributes["main"].Value;
                createAllPlugins(plugin_path, plugin_main);
            }

            foreach (Domain domain in m_domains)
            {
                EditorGraph.registerComponent(null, domain);
                domain.init(render_sys);                
            }

            return true;
        }

        private void createAllPlugins(string plugin_path, string plugin_main)
        {
            string[] dll_names = Directory.GetFiles(plugin_path, "*.dll");
            foreach(string name in dll_names)
            {
                CommonTool.newLog(this, "加载插件:" + name);
                bool succeed = false;
                try
                {
                    Assembly assembly = Assembly.LoadFrom(name);
                    
                    Type[] types = assembly.GetExportedTypes();
                    foreach (Type t in types)   //  找到domain
                    {
                        if(t.BaseType != null && t.BaseType == typeof(Domain))
                        {
                            Domain new_domain = (Domain)(assembly.CreateInstance(t.FullName));
                            if(new_domain != null)
                            {
                                m_domains.Add(new_domain);
                                if (new_domain.getName() == plugin_main)
                                {
                                    if (m_main_domain != null)
                                        m_main_domain.Active = false;
                                    
                                    m_main_domain = new_domain;
                                    m_main_domain.Active = true;
                                }
                                succeed = true;
                            }
                        }

                    }
                }
                catch (System.Exception){ }   

                string msg = succeed ? "加载成功:" + name : "加载失败:" + name;
                CommonTool.newLog(this, msg);
            }
        }

        public override NotifyResult notified(EditorComponent from, CastMessage msg)
        {
            return NotifyResult.Succeed;
        }

        public void release()
        {
            foreach (Domain domain in m_domains)
            {
                domain.release();
            }
        }

        public Domain MainDomain 
        {
            get { return m_main_domain; }
            set { m_main_domain = value; }
        }
        public List<Domain> Domains { get { return m_domains; } }

        Domain m_main_domain = null;
        List<Domain> m_domains = new List<Domain>();
        Editor_Form m_base_form = null;

        public static string ConfigPath = "../data/editor/editor_config.xml";
    }
}
