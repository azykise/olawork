using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using editor;

namespace modelview.ui
{
    public partial class NewMatForm : Form
    {
        public NewMatForm()
        {
            InitializeComponent();
            this.Hide();
        }

        public string VarName = "";
        public string VarPath = "";
        public string VarType = "";

        private void buttonPath_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog of = new System.Windows.Forms.OpenFileDialog();
            of.Title = "选择纹理";
            of.ShowReadOnly = true;
            of.Filter = "TGA Files(*.tga;)|*.tga;";
            of.FilterIndex = 1;
            of.RestoreDirectory = true;
            of.InitialDirectory = "../assets/";

            if (of.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                AssetPathMng path_mng = new AssetPathMng();
                this.VarPath = path_mng.ConvertToAssetPath(of.FileName);
                this.buttonPath.Text = this.VarPath;
            }         
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;

            VarName = this.textBox1.Text;
            VarPath = this.buttonPath.Text;
            
            if (this.radioButtonBool.Checked)
            {
                VarType = "bool";
            }
            else if (this.radioButtonTexture.Checked)
            {
                VarType = "texture";
            }
            else if (this.radioButtonVector4.Checked)
            {
                VarType = "vec4";
            }
        }

        private void NewMatForm_FormClosing(object sender, FormClosingEventArgs e)
        {            
        }

        private void radioButtonTexture_CheckedChanged(object sender, EventArgs e)
        {
            if (mLock)
                return;

            this.buttonPath.Enabled = radioButtonTexture.Checked;
        }

        bool mLock = false;
        private void NewMatForm_VisibleChanged(object sender, EventArgs e)
        {
            
            if (this.Visible == true)
            {               
                mLock = true;
                this.textBox1.Text = "";
                this.buttonPath.Text = "Path";
                this.buttonPath.Enabled = false;
                this.radioButtonVector4.Checked = false;
                this.radioButtonTexture.Checked = false;
                this.radioButtonBool.Checked = true;
                mLock = false;
                return;
            }
        }
    }
}
