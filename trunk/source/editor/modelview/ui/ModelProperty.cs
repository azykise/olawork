using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace modelview.ui
{
    partial class ModelProperty : UserControl
    {
        public ModelProperty()
        {
            InitializeComponent();
        }

        ModelPropertyComponent m_editcomp = null;

        public ModelPropertyComponent EditorComponent
        {
            set { m_editcomp = value; }
            get { return m_editcomp; }
        }
    }
}
