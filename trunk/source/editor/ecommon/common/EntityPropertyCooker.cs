using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;

namespace editor
{
    //属性序列化器使用范例
    class PropertyExample
    {
        public PropertyExample()
        {
            mSerializer["固有色"] = new PropertySerializer("固有色", mColor, delegate(string name,object value) { mColor = (Color)value; });
        }
        public void toPropertyGrid(PropertyGrid grid)
        {
            grid.SelectedObject = new GridPropertys(mSerializer.Values);
        }
        public Dictionary<string, PropertySerializer> mSerializer = new Dictionary<string, PropertySerializer>();
        Color mColor = Color.FromArgb(255, 255, 0, 0);
    }

    class GridPropertyDesc : System.ComponentModel.PropertyDescriptor
    {
        public GridPropertyDesc(ref PropertySerializer property, Attribute[] attrs) :
            base(property.Name, attrs)
        {
            mProperty = property;
        }
        PropertySerializer mProperty = null;
        public override bool CanResetValue(object component) { return false; }
        public override Type ComponentType { get { return null; } }
        public override string Category { get { return mProperty.Category; } }
        public override object GetValue(object component) { return mProperty.Value; }
        public override bool IsReadOnly { get { return false; } }
        public override Type PropertyType { get { return mProperty.Value.GetType(); } }
        public override void ResetValue(object component) { }
        public override void SetValue(object component, object value) { mProperty.Value = value; }
        public override bool ShouldSerializeValue(object component) { return false; }
        public override System.ComponentModel.TypeConverter Converter
        {
            get { return mProperty.TypeConv == null ? base.Converter : mProperty.TypeConv; }
        }
        public override object GetEditor(Type editorBaseType)
        {
            return mProperty.Editor == null ? base.GetEditor(editorBaseType) : mProperty.Editor;
        }
    }

    public class GridPropertys : CollectionBase, ICustomTypeDescriptor
    {
        public GridPropertys(IEnumerable<PropertySerializer> descs)
        {
            foreach (PropertySerializer desc in descs)
            {
                base.List.Add(desc);

                if (typeof(Color) == desc.Value.GetType())
                {
                    desc.Editor = sColorEditor;
                }
                else if (typeof(string) == desc.Value.GetType())
                {
                    string s = (string)desc.Value;
                    if ((s.Contains("\\") || s.Contains("/")) && s.Contains("."))
                    {
                        desc.Editor = sFileEditor;
                    }
                }

            }
        }

        PropertySerializer this[int index]
        {
            get { return (PropertySerializer)base.List[index]; }
            set { base.List[index] = (PropertySerializer)value; }
        }

        static GridPropertyColorEditor sColorEditor = new GridPropertyColorEditor();
        static GridPropertyFileEditor sFileEditor = new GridPropertyFileEditor();

        #region ICustomTypeDescriptor 成员
        PropertyDescriptorCollection ICustomTypeDescriptor.GetProperties(Attribute[] attributes)
        {
            GridPropertyDesc[] descs = new GridPropertyDesc[this.Count];
            for (int i = 0; i < this.Count; i++)
            {
                PropertySerializer desc = this[i];
                descs[i] = new GridPropertyDesc(ref desc, attributes);
            }
            return new PropertyDescriptorCollection(descs);
        }
        AttributeCollection ICustomTypeDescriptor.GetAttributes() { return TypeDescriptor.GetAttributes(this, true); }
        string ICustomTypeDescriptor.GetClassName() { return TypeDescriptor.GetClassName(this, true); }
        string ICustomTypeDescriptor.GetComponentName() { return TypeDescriptor.GetComponentName(this, true); }
        TypeConverter ICustomTypeDescriptor.GetConverter() { return TypeDescriptor.GetConverter(this, true); }
        EventDescriptor ICustomTypeDescriptor.GetDefaultEvent() { return TypeDescriptor.GetDefaultEvent(this, true); }
        PropertyDescriptor ICustomTypeDescriptor.GetDefaultProperty() { return TypeDescriptor.GetDefaultProperty(this, true); }
        object ICustomTypeDescriptor.GetEditor(Type editorBaseType) { return TypeDescriptor.GetEditor(this, editorBaseType, true); }
        EventDescriptorCollection ICustomTypeDescriptor.GetEvents(Attribute[] attributes) { return TypeDescriptor.GetEvents(this, attributes, true); }
        EventDescriptorCollection ICustomTypeDescriptor.GetEvents() { return TypeDescriptor.GetEvents(this, true); }
        PropertyDescriptorCollection ICustomTypeDescriptor.GetProperties() { return TypeDescriptor.GetProperties(this, true); }
        object ICustomTypeDescriptor.GetPropertyOwner(PropertyDescriptor pd) { return this; }

        #endregion
    }

    class GridPropertyColorEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        static ColorDialog sColorDialog = new ColorDialog();

        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            if (sColorDialog == null)
                sColorDialog = new ColorDialog();

            sColorDialog.Color = Color.FromArgb(1, 128, 128, 128);
            sColorDialog.AllowFullOpen = true;
            sColorDialog.FullOpen = true;
            sColorDialog.AnyColor = true;

            if (sColorDialog.ShowDialog() == DialogResult.OK)
                return sColorDialog.Color;

            return value;
        }
    }

    class GridPropertyFileEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        static OpenFileDialog sOpenFileDialog = new OpenFileDialog();

        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            if (sOpenFileDialog == null)
                sOpenFileDialog = new OpenFileDialog();

            if (sOpenFileDialog.ShowDialog() == DialogResult.OK)
                return sOpenFileDialog.FileName;

            return value;
        }
    }
}
