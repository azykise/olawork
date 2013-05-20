using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

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
            sl.Add(new editor.PropertySerializer(MaterialInfo.SHADER_FILENAME, currentMat.mShaderFilename, materialPropertyChanged));

            foreach (KeyValuePair<string, MatVarInfo> kvp in currentMat.mMatVars)
            {
                sl.Add(new editor.PropertySerializer(kvp.Key, kvp.Value.info[2], materialPropertyChanged));
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

            MaterialInfo currentMat = mMats[mCurMesh];

            if (name == MaterialInfo.SHADER_FILENAME)
            {
                currentMat.mShaderFilename = MatTools.GetAssetFilePath(_value as string);
            } 
            else
            {
                MatVarInfo info = null;
                if (currentMat.mMatVars.TryGetValue(name, out info))
                {
                    info.info[2] = _value as string;
                }
                XmlDocument xml = currentMat.toXML();
                xml.Save(currentMat.mMatFilename);
            }

            
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

            if ( mMats[mCurMesh].mMatFilename == null ||
                 mMats[mCurMesh].mMatFilename.Length == 0 ||
                 mMats[mCurMesh].mMatFilename.Contains("default.mat"))
            {
                mNewMatForm.Owner = this.ParentForm;

                System.Windows.Forms.DialogResult result = mNewMatForm.ShowDialog();
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    string matpath = mNewMatForm.GetMatFullname();
                    mMats[mCurMesh].mMatFilename = editor.Tool.GetAssetsRelativeFileFullPath(matpath);
                    XmlDocument xml = mMats[mCurMesh].toXML();
                    xml.Save(matpath);

                    mDml.mSubmeshMats[mCurMesh] = mMats[mCurMesh].mMatFilename;
                    xml = mDml.toXML();
                    xml.Save(mDml.mDmlpath);
                } 
            }
        }

        private void comboMat_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboMat_SelectedValueChanged(object sender, EventArgs e)
        {
            mCurMesh = this.comboMat.SelectedItem as string;
            refreshUI(mCurMesh);
        }
    }
}
