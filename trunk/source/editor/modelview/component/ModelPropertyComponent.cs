using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace modelview
{
    using editor;
    class ModelPropertyComponent : EditorComponent
    {
        public ModelPropertyComponent(ModelViewDoc doc):
            base("ModelPropertyComponent")
        {
            Doc = doc;

            ModelPropertyUI = new modelview.ui.ModelProperty();
            ModelPropertyUI.EditorComponent = this;
        }

        public ModelViewDoc Doc = null;
        public modelview.ui.ModelProperty ModelPropertyUI = null;
    }
}
