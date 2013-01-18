using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace editor.ui
{
    public partial class ExpandPanel : UserControl
    {
        public ExpandPanel()
        {
            InitializeComponent();
        }

        bool mExpand = true;
        bool mLock = false;

        Size mBackOrgSize = new Size();

        public delegate void OnExpandChangeEvent(bool expand);

        [Browsable(true)]
        public event OnExpandChangeEvent OnExpandChange = null;

        [Browsable(true)]
        public string PanelText
        {
            get { return this.label1.Text; }
            set { this.label1.Text = value; }
        }

        [Browsable(true)]
        public bool Expand
        {
            get { return mExpand; }
            set {
                mExpand = value;
                if (mExpand)
                {  
                    this.buttonExpand.Text = "-";
                }
                else
                {
                    this.buttonExpand.Text = "+";
                }
                if (OnExpandChange != null)
                    OnExpandChange(mExpand);
            }
        }

        private void ExpandPanel_SizeChanged(object sender, EventArgs e)
        {
            if (mLock)
                return;
        }

        private void buttonExpand_Click(object sender, EventArgs e)
        {
            mLock = true;

            this.Expand = !this.mExpand;
      
            mLock = false;
        }
    }
}
