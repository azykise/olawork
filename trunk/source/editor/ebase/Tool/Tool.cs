using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using System.Drawing;
using Sharp3D.Math.Geometry3D;
using System.Windows.Forms;

namespace editor
{    
    public class Tool
    {
        static float s_DEG2RAD = (float)MathFunctions.PI / 180.0f;
        static float s_RAD2DEG = 180.0f / (float)MathFunctions.PI;

        public static float Deg2Rad(float deg)
        {
            return deg * s_DEG2RAD;
        }
        public static float Rad2Deg(float rad)
        {
            return rad * s_RAD2DEG;
        }

        public static List<System.Windows.Forms.TreeNode> FindTreeNodesByText(System.Windows.Forms.TreeView tree_view,string text)
        {
            List<System.Windows.Forms.TreeNode> result = new List<System.Windows.Forms.TreeNode>();
            foreach (System.Windows.Forms.TreeNode node in tree_view.Nodes)
            {
                if (node.Text == text)
                    result.Add(node);
                FindTreesNodeByText(node, text, result);
            }
            return result;
        }

        public static void FindTreesNodeByText(System.Windows.Forms.TreeNode tree_node, string text, List<System.Windows.Forms.TreeNode> result)
        {
            foreach (System.Windows.Forms.TreeNode node in tree_node.Nodes)
            {
                if (node.Text == text)
                    result.Add(node);
                FindTreesNodeByText(node, text, result);
            }
        }

        public static string GetAssetsRelativeFileFullPath(string abs_fullpath)
        {
            int i = abs_fullpath.IndexOf("assets");
            string s = abs_fullpath.Substring(i);
            string o = "..\\" + s;
            return o;
        }

        public static string GetFilename(string fullname,bool suffix = true)
        {
            string s = fullname.Replace('\\','/');
            int last_dot = s.LastIndexOf('.');
            int last_spr = s.LastIndexOf('/');

            if (last_spr == -1)
            {
                if (!suffix)
                {
                    if (last_dot != -1)
                    {
                        string r = s.Substring(0, last_dot);
                        return r;
                    }
                }
                return s;
            }
            else
            {
                last_spr += 1;
                if (!suffix)
                {
                    if (last_dot != -1 && last_dot > last_spr)
                    {
                        string r = s.Substring(last_spr, last_dot - last_spr);
                        return r;
                    }                
                }

                string _r = s.Substring(last_spr);
                return _r;

            }
        }

        public static string GetAssetAbsoultFilePath(string rel_path)
        {
            int i = rel_path.IndexOf("..\\");
            string s = rel_path.Substring(i + 3);
            string c = Environment.CurrentDirectory;
            int j = c.IndexOf("bin");
            string b = c.Substring(0, j);
            string o = b + s;
            return o;
        }

        public static string GetAssetFolderFullPath()
        {
            string exe_fullname = Application.ExecutablePath;
            int i = exe_fullname.IndexOf("bin");
            string s = exe_fullname.Substring(0,i);
            return s + "assets\\";
        }

        public static string GetFilePath(string fullname)
        {
            int i = fullname.LastIndexOf("\\");
            if (i == -1)
            {
                i = fullname.LastIndexOf("/");
            }
            return fullname.Substring(0, i + 1);
        }

        public static string GetFileSuffix(string filename)
        {
            int i = filename.LastIndexOf(".");
            if (i == -1 || i == filename.Length)
                return "";
            else
            {
                return filename.Substring(i + 1).ToLower();
            }
        }

        public static Point FormPointToScreen(Point formpt, int formw, int formh)
        {
            return new Point(formpt.X, formh - formpt.Y);
        }
        public static Point FormPointToScreen(Point formpt, View view)
        {
            return FormPointToScreen(formpt, view.RenderWindow.getWidth(), view.RenderWindow.getHeight());
        }

        public static Rectangle MakeScreenRect(Point screen_p1,Point screen_p2)
        {
            int left = Math.Min(screen_p1.X, screen_p2.X);
            int right = Math.Max(screen_p1.X, screen_p2.X);
            int top = Math.Min(screen_p1.Y, screen_p2.Y);
            int buttom = Math.Max(screen_p1.Y, screen_p2.Y);
            Rectangle r = new Rectangle(left,top,right - left,buttom - top);
            return r;
        }

        public static IntersectionPair Intersect(Point screenpt, Plane plane, View view)
        {
            Vector3F pos = view.getCamera().getScreenWorldLocation(screenpt.X, screenpt.Y, view);
            Vector3F dir = pos - view.getCamera().Position; dir.Normalize();

            Ray ray = new Ray(pos, dir);            
            IntersectionPair res = Sharp3D.Math.Geometry3D.IntersectionMethods.Intersects(ray, plane);
            return res;
        }

        public static float GetElaspedTime(ref long last_tick)
        {
            if (last_tick == 0)
            {
                last_tick = DateTime.Now.Ticks / 10000;
                return 0;
            }
            else
            {
                long tnow = DateTime.Now.Ticks / 10000;
                long dt = tnow - last_tick;

                float dtf = (float)dt / 1000.0f;

                last_tick = tnow;

                return dtf;
            }
        }     

        public static EditorDoc GetCurrentDoc(EditorComponent component)
        {
            Domain domain = DomainManager.Instance().getDomain(component);
            return domain == null ? null : domain.getCurrentDoc();
        }

        public static object GetObjectBasedOnStrType(string type,string value)
        {
            switch(type)
            {
                case TYPE_FLOAT:
                    return float.Parse(value);
                case TYPE_INT:
                    return int.Parse(value);
                case TYPE_VEC3:
                    return Vector3F.Parse(value);
                case TYPE_VEC4:
                    return Vector4F.Parse(value);
                case TYPE_STRING:
                    return value;
                case TYPE_LONG:
                    return Int64.Parse(value);
            }
            return null;
        }

        public const string TYPE_FLOAT = "float";
        public const string TYPE_INT = "int";
        public const string TYPE_LONG = "long";
        public const string TYPE_VEC3 = "vec3";
        public const string TYPE_VEC4 = "vec4";
        public const string TYPE_STRING = "string";
        public const string TYPE_ENUM = "enum";

        public const string SUFFIX_ASE = "ase";
        public const string SUFFIX_DML = "dml";
        public const string SUFFIX_CHR = "chr";
    }

    public class SphereSpace
    {
        public SphereSpace()
        {

        }

        public SphereSpace(float _radius,float _dy,float _dz)
        {
            radius = _radius;
            degreeY = _dy;
            degreeZ = _dz;
        }

        public Vector3F orthPosition(float degreez, float degreey)
        {
            degreeZ = degreez;
            degreeY = degreey;
            return orthPosition();
        }

        public void fromOrth(Vector3F p)
        {
            radius = p.GetLength();
            
            float ga = (float)Math.Atan(p.Y / p.X);
            degreeY = Tool.Rad2Deg(ga);

            float th = (float)Math.Acos(p.Z / radius);
            degreeZ = Tool.Rad2Deg(th);
        }

        public Vector3F orthPosition()
        {
            float th = Tool.Deg2Rad(degreeZ);
            float ga = Tool.Deg2Rad(degreeY);

            Vector3F p = new Vector3F();

            p.Y = MathFunctions.Sin(th) * radius;
            p.Y *= MathFunctions.Cos(ga);

            p.X = MathFunctions.Sin(th) * radius;
            p.X *= MathFunctions.Sin(ga);

            p.Z = MathFunctions.Cos(th) * radius;

            return p;
        }

        public float radius = 1.0f;
        public float degreeY = 0.0f;

        public float degreeZ = 1.0f;
    }
}
