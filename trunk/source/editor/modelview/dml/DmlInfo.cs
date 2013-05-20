using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections.Generic;
using editor;

namespace modelview
{
    class DmlInfo
    {
        public string mDmlpath = "";
        public string mASE = "";
        public Dictionary<string,string> mSubmeshMats = new Dictionary<string,string>();

        public void fromXML(string dmlfilename)
        {
            mDmlpath = dmlfilename;

            XmlDocument xml = new XmlDocument();
            xml.Load(dmlfilename);

            XmlNode root = xml.FirstChild;

            XmlNode geom = root.SelectSingleNode("geometry");
            mASE = geom.Attributes["resource"].Value;

            XmlNode mat = root.SelectSingleNode("material");
            XmlNodeList varNodes = mat.SelectNodes("submat");

            foreach (XmlNode varnode in varNodes)
            {
                string[] ss = new string[2];
                ss[0] = varnode.Attributes["name"].Value;
                ss[1] = varnode.Attributes["resource"].Value;
                mSubmeshMats[ss[0]] = "../" + MatTools.ASSET_PATH_MARK + (ss[1]);
            }
        }

        public XmlDocument toXML()
        {
            XmlDocument xml = new XmlDocument();

            XmlNode root = xml.CreateNode("element", "ola_model", "");

            XmlNode geom = xml.CreateNode("element", "geometry", "");
            root.AppendChild(geom);
            XmlAttribute att = xml.CreateAttribute("resource");
            att.Value = mASE;
            geom.AppendChild(att);

            XmlNode mat = xml.CreateNode("element", "material", "");
            root.AppendChild(mat);
            att = xml.CreateAttribute("num");
            att.Value = mSubmeshMats.Keys.Count.ToString();
            mat.AppendChild(att);

            int n = 0;
            foreach (KeyValuePair<string, string> kvp in mSubmeshMats)
            {
                XmlNode varnode = xml.CreateNode("element", "submat", "");

                att = xml.CreateAttribute("index");
                att.Value = n.ToString();
                varnode.AppendChild(att);

                att = xml.CreateAttribute("name");
                att.Value = kvp.Key;
                varnode.AppendChild(att);

                att = xml.CreateAttribute("resource");
                att.Value = kvp.Value;
                varnode.AppendChild(att);

                mat.AppendChild(varnode);

                n += 1;
            }

            return xml;
        }
    }
}
