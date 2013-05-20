using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace modelview
{
    class SubMeshInfo
    {
        public string mName = "";
    }

    class MatVarInfo
    {
        public MatVarInfo(string[] _i)
        {
            info = _i;
        }
        public string[] info = new string[] { "name" , "type" ,"value"};
    }

    class MaterialInfo
    {
        public const string MATERIAL_FILENAME = "Material";
        public const string SHADER_FILENAME = "Shader";

        public string mMatFilename = "";
        public string mShaderFilename = "default.glsl";

        public Dictionary<string, MatVarInfo> mMatVars = new Dictionary<string, MatVarInfo>();
        

        public void fromXML(string matfilename)
        {
            mMatFilename = matfilename;

            XmlDocument xml = new XmlDocument();
            xml.Load(matfilename);

            XmlNode root = xml.FirstChild;
            XmlNode inst = root.FirstChild;

            XmlNode tech = inst.SelectSingleNode("technique");
            mShaderFilename = "../asset/" + tech.Attributes["source_s"].Value;

            XmlNodeList varNodes = inst.SelectNodes("var");
            List<string[]> vars = new List<string[]>();
            foreach (XmlNode varnode in varNodes)
            {
                string[] ss = new string[3];
                ss[0] = varnode.Attributes["name"].Value;
                ss[1] = varnode.Attributes["type"].Value;
                ss[2] = varnode.Attributes["value"].Value;
                vars.Add(ss);
            }

            for (int i = 0; i < vars.Count; i++)
            {
                mMatVars[vars[i][0]] = new MatVarInfo(vars[i]);
            }
        }

        public XmlDocument toXML()
        {
            XmlDocument xml = new XmlDocument();

            XmlNode root = xml.CreateNode("element", "ola_material", "");
            xml.AppendChild(root);
            XmlAttribute att = xml.CreateAttribute("version");
            att.Value = "1.0";
            root.AppendChild(att);

            XmlNode inst = xml.CreateNode("element", "instance", "");
            root.AppendChild(inst);
            att = xml.CreateAttribute("name");
            att.Value = editor.Tool.GetFilename(mMatFilename);
            inst.AppendChild(att);

            XmlNode tech = xml.CreateNode("element", "technique", "");
            inst.AppendChild(tech);
            att = xml.CreateAttribute("type");
            att.Value = "shader";
            tech.AppendChild(att);
            att = xml.CreateAttribute("source_s");
            att.Value = MatTools.GetAssetFilePath(mShaderFilename);
            tech.AppendChild(att);

            foreach (MatVarInfo varinfo in mMatVars.Values)
            {
                XmlNode varnode = xml.CreateNode("element", "var", "");

                att = xml.CreateAttribute("name");
                att.Value = varinfo.info[0];
                varnode.AppendChild(att);

                att = xml.CreateAttribute("type");
                att.Value = varinfo.info[1];
                varnode.AppendChild(att);

                att = xml.CreateAttribute("value");
                att.Value = varinfo.info[2];
                varnode.AppendChild(att);

                inst.AppendChild(varnode);
            }

            return xml;
        }
    }

    class MatTools
    {
        public const string ASSET_PATH_MARK = "assets/";
        public static string GetAssetFilePath(string anypath)
        {
            string s = anypath.ToLower();
            s = s.Replace('\\', '/');
            
            int index = s.IndexOf(ASSET_PATH_MARK);
            if (index == -1)
            {
                if (!s.Contains(":"))
                    return "../" + ASSET_PATH_MARK + anypath;
                else
                    return "";
            }

            string r = s.Substring(index + ASSET_PATH_MARK.Length);
            return r;
        }
    }
}
