using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using editor;

namespace modelview.ui
{
    partial class ModelMatList : UserControl
    {
        public ModelMatList()
        {
            InitializeComponent();
        }

        public void initializeUI(DmlInfo dml)
        {
            this.comboMat.Items.Clear();
            mMats.Clear();
            mDml = dml;

            foreach (KeyValuePair<string, string> kvp in dml.mSubmeshMats)
            {
                MaterialInfo mat = new MaterialInfo();
                mat.mMatFilename = kvp.Value;
                if(!mat.mMatFilename.Contains("default"))
                    mat.fromXML(mat.mMatFilename);
                mMats[kvp.Key] = mat;
            }

            if (mMats.Count == 0)
            {
                refreshUI(null);
                return;
            }

            this.comboMat.Items.AddRange(mMats.Keys.ToArray<string>());
            this.comboMat.SelectedItem = this.comboMat.Items[0];
            mCurMesh = this.comboMat.SelectedItem as string;
            refreshUI(mCurMesh);
        }

        public void refreshUI(string curMesh)
        {
            mLock = true;

            MaterialInfo currentMat = mMats[curMesh];

            this.buttonMat.Text = "";
            this.propertyMat.SelectedObject = null;

            if (currentMat == null)
            {
                return;
            }

            this.buttonMat.Text = editor.Tool.GetFilename(currentMat.mMatFilename, false);

            List<editor.PropertySerializer> sl = new List<editor.PropertySerializer>();
            editor.PropertySerializer psr = new editor.PropertySerializer(MaterialInfo.SHADER_FILENAME, currentMat.mShaderFilename, materialPropertyChanged);
            psr.Editor = GridPropertys.sFileEditor;
            sl.Add(psr);

            foreach (KeyValuePair<string, MatVarInfo> kvp in currentMat.mMatVars)
            {                
                psr = new editor.PropertySerializer(kvp.Key, kvp.Value.info[2], materialPropertyChanged);
                if (kvp.Value.info[1] == "texture")
                {
                    psr.Editor = GridPropertys.sFileEditor;
                }
                sl.Add(psr);
            }

            this.propertyMat.SelectedObject = new editor.GridPropertys(sl);

            mLock = false;
        }

        protected void materialPropertyChanged(string name, object _value)
        {
            if (mCurMesh == null || mLock)
            {
                return;
            }

            AssetPathMng path_mng = new AssetPathMng();

            MaterialInfo currentMat = mMats[mCurMesh];
            string disk_matfilename = path_mng.ConvertToDiskPath(currentMat.mMatFilename);

            if (name == MaterialInfo.SHADER_FILENAME)
            {
                string glsl_path = _value as string;

                if (glsl_path.Contains(".glsl") || glsl_path.Contains(".shader"))
                {
                    currentMat.mShaderFilename = path_mng.ConvertToAssetPath(glsl_path);

                    XmlDocument xml = currentMat.toXML();
                    xml.Save(disk_matfilename);
                }
            } 
            else
            {
                MatVarInfo info = null;
                if (currentMat.mMatVars.TryGetValue(name, out info))
                {
                    info.info[2] = path_mng.ConvertToAssetPath(_value as string);
                }
                XmlDocument xml = currentMat.toXML();
                xml.Save(disk_matfilename);
            }

            refreshUI(mCurMesh);
        }

        string mCurMesh = null;
        Dictionary<string, MaterialInfo> mMats = new Dictionary<string, MaterialInfo>();

        NewMatForm mNewMatForm = new NewMatForm();
        DmlInfo mDml = null;

        public MaterialListComponent EditorComponent = null;
        bool mLock = false;

        private void buttonMat_Click(object sender, EventArgs e)
        {
            if (mCurMesh == null)
            {
                return;
            }

            string matpath = "";
            AssetPathMng path_mng = new AssetPathMng();

            if (mMats[mCurMesh].mMatFilename == null ||
                 mMats[mCurMesh].mMatFilename.Length == 0 ||
                 mMats[mCurMesh].mMatFilename.Contains("default.mat"))
            {
                string dml = mDml.mDmlpath;
                string folder = editor.Tool.GetFilePath(dml);
                matpath = folder + mCurMesh + ".mat";

                
                mMats[mCurMesh].mMatFilename = path_mng.ConvertToAssetPath(matpath);

                string disk_matpath = path_mng.ConvertToDiskPath(matpath);
                if(System.IO.File.Exists(disk_matpath))
                {
                    mMats[mCurMesh].fromXML(mMats[mCurMesh].mMatFilename);
                }
                else
                {
                    XmlDocument matxml = mMats[mCurMesh].toXML();
                    matxml.Save(disk_matpath);
                }

                mDml.mSubmeshMats[mCurMesh] = mMats[mCurMesh].mMatFilename;
                XmlDocument xml = mDml.toXML();

                string disk_dmlpath = path_mng.ConvertToDiskPath(mDml.mDmlpath);
                xml.Save(disk_dmlpath);
            }
            else
            {
                System.Windows.Forms.OpenFileDialog of = new System.Windows.Forms.OpenFileDialog();
                of.Title = "打开...";
                of.ShowReadOnly = true;
                of.Filter = "Mat Files(*.mat)|*.mat";
                of.FilterIndex = 1;
                of.RestoreDirectory = true;
                of.InitialDirectory = "../assets/";

                if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    matpath = of.FileName;
                }
                else
                    return;

                mMats[mCurMesh].mMatFilename = path_mng.ConvertToAssetPath(matpath);
                mMats[mCurMesh].fromXML(matpath);

                mDml.mSubmeshMats[mCurMesh] = mMats[mCurMesh].mMatFilename;
                XmlDocument xml = mDml.toXML();

                string disk_dmlpath = path_mng.ConvertToDiskPath(mDml.mDmlpath);
                xml.Save(disk_dmlpath);
            }

            refreshUI(mCurMesh);
        }

        private void comboMat_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboMat_SelectedValueChanged(object sender, EventArgs e)
        {
            mCurMesh = this.comboMat.SelectedItem as string;
            refreshUI(mCurMesh);
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (mCurMesh == null)
            {
                return;
            }

            DialogResult r = mNewMatForm.ShowDialog();
            if (r == DialogResult.OK)
            {
                if (mNewMatForm.VarName == null || mNewMatForm.VarName.Length == 0 ||
                    mNewMatForm.VarType == null || mNewMatForm.VarType.Length == 0)
                    return;

                string[] ss = new string[3];
                ss[0] = mNewMatForm.VarName;
                ss[1] = mNewMatForm.VarType;

                switch (mNewMatForm.VarType)
                {
                    case "bool":
                        ss[2] = "false";
                        break;
                    case "texture":
                        if (mNewMatForm.VarPath != null && mNewMatForm.VarPath.Length != 0 && mNewMatForm.VarPath.ToLower() != "path")
                            ss[2] = mNewMatForm.VarPath;
                        else
                            ss[2] = "default.tga";
                        break;
                    case "vec4":
                        ss[2] = "0,0,0,0";
                        break;
                    default:
                        return;
                }
                
                MatVarInfo var = new MatVarInfo(ss);
                mMats[mCurMesh].mMatVars[var.info[0]] = var;

                AssetPathMng path_mng = new AssetPathMng();
                XmlDocument xml = mMats[mCurMesh].toXML();
                string disk_matfilename = path_mng.ConvertToDiskPath(mMats[mCurMesh].mMatFilename);
                xml.Save(disk_matfilename);

                refreshUI(mCurMesh);
            }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if (mCurMesh == null)
            {
                return;
            }

            if (mMats[mCurMesh].mMatVars.ContainsKey(this.propertyMat.SelectedGridItem.Label))
            {
                mMats[mCurMesh].mMatVars.Remove(this.propertyMat.SelectedGridItem.Label);

                AssetPathMng path_mng = new AssetPathMng();
                XmlDocument xml = mMats[mCurMesh].toXML();
                string disk_matfilename = path_mng.ConvertToDiskPath(mMats[mCurMesh].mMatFilename);
                xml.Save(disk_matfilename);

                refreshUI(mCurMesh);
            }
        }
    }
}
