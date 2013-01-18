using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    //可编辑的图元对象
    public abstract class ScenePrimitive : Entity
    {
        protected void setBasePrimitive(Engine.Primitive pri)
        {
            mPrimitive = pri;
        }
        public override bool pushToSelect(uint id, Engine.RenderSystem rendersys)
        {
            if(mPrimitive != null)
                rendersys.pushToSelect(mPrimitive, id);
            return true;
        }

        public override void render(Engine.RenderSystem rendersys)
        {
            if(mPrimitive != null)
                mPrimitive.render(rendersys);
        }
       
        private Engine.Primitive mPrimitive = null; //基类专用 子类可以保留具体类型的primitive
    }

    //public class SceneBorderCubeMemento : EntityMemento
    //{
    //    public SceneBorderCubeMemento(SceneBorderCube entity):
    //        base(entity)
    //    {
    //        mSceneBorderCube = entity;
    //        mMemento[NAME_SIZE] = new EntityProperty(NAME_SIZE, "", NAME_SIZE, new Vector3F(10, 10, 10), EntityProperty.TYPE_VEC3);
    //    }

    //    public override void getFromEntity()
    //    {
    //        base.getFromEntity();
    //        recordOneProperty(NAME_SIZE, mSceneBorderCube.Size);
    //    }

    //    public override void setToEntity()
    //    {
    //        base.setToEntity();
    //        mSceneBorderCube.Size = getProperty<Vector3F>(NAME_SIZE);
    //    }

    //    public const string NAME_SIZE = "长宽高度";

    //    SceneBorderCube mSceneBorderCube = null;
    //}

    public class SceneBorderCube : ScenePrimitive
    {
        public const string CLASS_NAME = "SceneBorderCube";

        public SceneBorderCube(string name,Engine.RenderSystem rendersys)
        {
            mName = name;
            mCube = rendersys.createPrimitiveBorderCube(new Vector3F(0.0f, 0.0f, 0.0f), 10.0f, 10.0f, 10.0f);
            mID = mCube.getID();
            //mMemento = new SceneBorderCubeMemento(this);
            setBasePrimitive(mCube);
        }

        //public override EntityMemento getMemento()
        //{
        //    return mMemento;
        //}

        public override Vector3F Position
        {
            get{return base.Position;}
            set{
                base.Position = value;
                mCube.setBorderCube(mPosition, mSize.X, mSize.Y, mSize.Z);
            }
        }

        public override bool Selected
        {
            get{return base.Selected;}
            set
            {
                base.Selected = value;
                if (mSelected)
                    mCube.setColor(1.0f, 1.0f, 0.0f, 1.0f);
                else
                    mCube.setColor(0.6f, 0.6f, 0.6f, 1.0f);

            }
        }

        public Vector3F Size
        {
            get { return mSize; }
            set
            {
                mSize = value;
                //mMemento.recordOneProperty(SceneBorderCubeMemento.NAME_SIZE, value);
                mCube.setBorderCube(mPosition, mSize.X, mSize.Y, mSize.Z);
            }
        }

        Vector3F mSize = new Vector3F(10.0f, 10.0f, 10.0f);
        Engine.PrimitiveBorderCube mCube = null;
    }

    //public class ScenePlaneMemento : EntityMemento
    //{
    //    public ScenePlaneMemento(ScenePlane entity) :
    //        base(entity)
    //    {
    //        mScenePlane = entity;
    //        mMemento[NAME_NORMAL] = new EntityProperty(NAME_NORMAL, "", NAME_NORMAL, new Vector3F(0, 0, 1), EntityProperty.TYPE_VEC3);
    //        mMemento[NAME_SIZE] = new EntityProperty(NAME_SIZE, "", NAME_SIZE, new Vector2F(10, 10), EntityProperty.TYPE_VEC2);
    //    }

    //    public override void getFromEntity()
    //    {
    //        base.getFromEntity();
    //        recordOneProperty(NAME_SIZE, mScenePlane.Size);
    //    }

    //    public override void setToEntity()
    //    {
    //        base.setToEntity();
    //        mScenePlane.Size = getProperty<Vector2F>(NAME_SIZE);
    //    }

    //    public const string NAME_NORMAL = "平面朝向";
    //    public const string NAME_SIZE = "平面长宽";

    //    ScenePlane mScenePlane = null;
    //}

    public class ScenePlane : ScenePrimitive
    {
        public const string CLASS_NAME = "ScenePlane";
        public ScenePlane(string name, Engine.RenderSystem rendersys)
        {
            mName = name;
            mPlane = rendersys.createPrimitivePlane(new Vector3F(0.0f, 0.0f, 0.0f), mNormal, mSize.X, mSize.Y);
            mID = mPlane.getID();
            //mMemento = new ScenePlaneMemento(this);
            setBasePrimitive(mPlane);
        }

        //public override EntityMemento getMemento()
        //{
        //    return mMemento;
        //}

        public override bool Selected
        {
            get { return base.Selected; }
            set
            {
                base.Selected = value;
                if (mSelected)
                    mPlane.setColor(1.0f, 1.0f, 0.0f, 1.0f);
                else
                    mPlane.setColor(0.6f, 0.6f, 0.6f, 1.0f);
            }
        }

        public override Vector3F Position
        {
            get { return base.Position; }
            set
            {
                base.Position = value;
                mPlane.setPlane(mPosition, mNormal, mSize.X, mSize.Y);
            }
        }

        public Vector2F Size 
        {
            get { return mSize; }
            set 
            {
                mSize = value;
                mPlane.setPlane(mPosition, mNormal, mSize.X, mSize.Y);
                //mMemento.recordOneProperty(ScenePlaneMemento.NAME_SIZE, value);
            }
        }

        public Vector3F Normal
        {
            get { return mNormal; }
            set 
            {
                mNormal = value;
                mPlane.setPlane(mPosition, mNormal, mSize.X, mSize.Y);
                //mMemento.recordOneProperty(ScenePlaneMemento.NAME_NORMAL, value);
            }
        }

        Vector2F mSize = new Vector2F(10, 10);
        Vector3F mNormal = new Vector3F(0, 0, 1);
        Engine.PrimitivePlane mPlane = null;
    }

    public class PrimitiveFactory : IEntityFactory
    {
        public PrimitiveFactory()
        {
            mTypeList.Add(SceneBorderCube.CLASS_NAME);
            mEntityCreateCount[SceneBorderCube.CLASS_NAME] = 0;

            mTypeList.Add(ScenePlane.CLASS_NAME);
            mEntityCreateCount[ScenePlane.CLASS_NAME] = 0;
        }

        public void initialize(string config_path,Engine.RenderSystem rendersys)
        {
            mRendersys = rendersys;
        }

        public Entity createEntity(string type, Dictionary<string, object> propertys, Engine.RenderSystem rendersys)
        {
            //EntityMemento memento = null;
            Entity entity = null;
            switch (type)
            {
                case SceneBorderCube.CLASS_NAME:
                    string bname = SceneBorderCube.CLASS_NAME + mEntityCreateCount[SceneBorderCube.CLASS_NAME].ToString();
                    SceneBorderCube cube = new SceneBorderCube(bname, rendersys);
                    //memento = new SceneBorderCubeMemento(cube);
                    entity = cube;
                    mEntityCreateCount[SceneBorderCube.CLASS_NAME] += 1;
                    break;
                case ScenePlane.CLASS_NAME:
                    string pname = ScenePlane.CLASS_NAME + mEntityCreateCount[ScenePlane.CLASS_NAME].ToString();
                    ScenePlane plane = new ScenePlane(pname, rendersys);
                    //memento = new ScenePlaneMemento(plane);
                    entity = plane;
                    mEntityCreateCount[ScenePlane.CLASS_NAME] += 1;
                    break;
            }

            //if (memento != null)
            //{
            //    foreach (KeyValuePair<string, object> kvp in propertys)
            //    {
            //        memento.recordOneProperty(kvp.Key, kvp.Value);
            //    }

            //    memento.setToEntity();
            //}

            return entity;
        }              

        public List<string> getSupportTypes(){return mTypeList;}

        List<string> mTypeList = new List<string>();

        Engine.RenderSystem mRendersys = null;

        Dictionary<string, int> mEntityCreateCount = new Dictionary<string, int>();
    }
}
