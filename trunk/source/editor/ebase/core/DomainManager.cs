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
    public class DomainManager : EditorComponent
    {
        public static DomainManager Instance()
        {
            if (s_domainmng == null)
            {
                s_domainmng = new DomainManager();
                return s_domainmng;
            }
            else
            {
                if (s_domainmng.m_base_form == null)
                {
                    throw new System.Exception("初始化DomainManager未赋予Editor_Form");
                }
                return s_domainmng;
            }

        }
        static DomainManager s_domainmng = null;
        long mElaspedRec = 0;


        public void setBaseForm(Form base_form)
        {
            m_base_form = base_form;
        }

        DomainManager() :
            base("DomainManager")
        {
           
        }

        public void loop()
        {
            if (mElaspedRec == 0)
            {
                mElaspedRec = DateTime.Now.Ticks / 10000;
            }

            long now_ms = DateTime.Now.Ticks / 10000;
            float elasped = (float)(now_ms - mElaspedRec) / 1000.0f;
            mElaspedRec = now_ms;

            foreach (Domain domain in m_domains)
            {
                if (domain.Active)
                {
                    domain.getViewManager().update(elasped);
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

        public bool init(string info, RenderSystem render_sys)
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

            EditorGraph.broadCast(this, new MessageBase(MessageType.NewDoc, ""));

            return true;
        }

        private void createAllPlugins(string plugin_path, string plugin_main)
        {
            string[] dll_names = Directory.GetFiles(plugin_path, "*.dll");
            foreach(string name in dll_names)
            {
                CommonTool.newLog(this, "加载插件:" + name);
                bool succeed = false;

                Assembly assembly = Assembly.LoadFrom(name);

                if (assembly == null)
                    goto CreateEnd;

                Type[] types = assembly.GetExportedTypes();
                foreach (Type t in types)   //  找到domain
                {
                    if(t.BaseType != null && t.BaseType == typeof(Domain))
                    {
                        try
                        {
                            Domain new_domain = (Domain)(assembly.CreateInstance(t.FullName));
                            if (new_domain != null)
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
                        catch (System.Exception) { }
                    }

                }

CreateEnd:
                string msg = succeed ? "加载成功:" + name : "加载失败:" + name;
                CommonTool.newLog(this, msg);
            }
        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
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

        public Domain getDomain(EditorComponent comp)
        {
            List<EditorComponent> parents = EditorGraph.getParents(comp);
            foreach (EditorComponent parent in parents)
            {
                Domain domain = parent as Domain;
                if (domain != null && m_domains.Contains(domain))
                {
                    return domain;
                }
            }
            return null;
        }

        public Domain MainDomain 
        {
            get { return m_main_domain; }
            set { m_main_domain = value; }
        }
        public List<Domain> Domains { get { return m_domains; } }

        Domain m_main_domain = null;
        List<Domain> m_domains = new List<Domain>();
        Form m_base_form = null;

        public static string ConfigPath = "./plugins/editor/editor_config.xml";
    }
}
