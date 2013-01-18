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
    using editor;
    public partial class CommonPropertyGrid : UserControl
    {
        public CommonPropertyGrid()
        {
            InitializeComponent();
        }

        public void refreshPropertyGrid()
        {

        }

        private void propertyGridEx1_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {

        }

        Dictionary<string, object> m_cache = new Dictionary<string, object>();
    }
}
