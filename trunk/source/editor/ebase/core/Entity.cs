using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    //一个属性的描述类 用来将这个属性序列化
    public class PropertySerializer
    {
        public delegate void ValueChanged(object value);
        public PropertySerializer(string name, object value, ValueChanged handler)
        {
            Name = name;
            mValue = value;
            Handle = handler;
        }

        public PropertySerializer(string name, string cate, string desc, object value)
        {
            Name = name;
            Category = cate;
            Desc = desc;
            mValue = value;
        }
        public PropertySerializer(string name, string cate, string desc, object value, ValueChanged handler)
        {
            Name = name;
            Category = cate;
            Desc = desc;
            mValue = value;
            Handle = handler;
        }

        public XmlNode toXml(XmlDocument xml)
        {
            XmlNode node = xml.CreateNode("element", "Property", "");
            XmlAttribute name_att = xml.CreateAttribute("name");
            name_att.Value = Name;
            node.Attributes.Append(name_att);
            return node;
        }
        public void fromXml(XmlNode node) { }

        public object Value
        {
            get { return mValue; }
            set { mValue = value; if (Handle != null) Handle(mValue); }
        }

        public string Name = "";
        public string Desc = "";
        public string Category = "属性";
        public object Editor = null;
        public ValueChanged Handle = null;
        public System.ComponentModel.TypeConverter TypeConv = null;

        public object mValue = null;
    }

    //movable object 可移动 有体积的东西
    public abstract class Entity 
    {
        public Entity()
        {

        }

        public virtual void update(float elaspedTime)
        {

        }

        public virtual void render(Engine.RenderSystem rendersys)
        {

        }

        public virtual Dictionary<string, PropertySerializer> Serializers { get { return mSerializers; } }
        
        public virtual UInt64 ID 
        {
            get { return mID; }
            set 
            {
                mID = value;
                //mMemento.recordOneProperty(EntityMemento.NAME_ID, value);
            }
        }

        public virtual string Name
        {
            get { return mName; }
            set 
            {
                mName = value;
                //mMemento.recordOneProperty(EntityMemento.NAME_NAME, value);
            }
        }

        public virtual Vector3F Position
        {
            get { return mPosition; }
            set 
            {
                mPosition = value;
                //mMemento.recordOneProperty(EntityMemento.NAME_POSITION, value);
            }
        }

        public virtual AxisAlignedBox AABB
        {
            get { return mAABB; }
            set { mAABB = value; }
        }

        public virtual EntityLayer Layer
        {
            get { return mLayer; }
            set { mLayer = value; }
        }

        public virtual bool Selected
        {
            get { return mSelected; }
            set { mSelected = value; }
        }

        public virtual bool pushToSelect(uint id,Engine.RenderSystem rendersys)
        {
            return false;
        }

        protected EntityLayer       mLayer = null;
        protected bool              mSelected = false;
        protected string            mName = "";
        protected Vector3F          mPosition = new Vector3F(0,0,0);
        protected QuaternionF       mRotation = new QuaternionF(1,0,0,0);
        protected Vector3F          mScale = new Vector3F(1,1,1);
        protected AxisAlignedBox    mAABB = new AxisAlignedBox();
        protected UInt64            mID = 0;

        protected Dictionary<string, PropertySerializer> mSerializers = new Dictionary<string, PropertySerializer>();
    }
}
