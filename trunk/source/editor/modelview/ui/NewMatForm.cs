using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace modelview.ui
{
    public partial class NewMatForm : Form
    {
        public NewMatForm()
        {
            InitializeComponent();
            this.Hide();
        }

        private void buttonPath_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            System.Windows.Forms.DialogResult result = folderBrowserDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                this.buttonPath.Text = folderBrowserDialog.SelectedPath;
            }          
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (this.textBox1.Text == null ||
                this.textBox1.Text.Length == 0 ||
                !this.textBox1.Text.Contains(".mat"))
            {
                return;
            }

            if (!System.IO.Directory.Exists(this.buttonPath.Text))
            {
                return;
            }

            this.DialogResult = DialogResult.OK;

            this.Hide();
        }

        public string GetMatFullname()
        {
            string s = this.buttonPath.Text.ToLower().Replace('\\','/');
            return s + "/" + this.textBox1.Text;
        }

        private void NewMatForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }
    }
}
