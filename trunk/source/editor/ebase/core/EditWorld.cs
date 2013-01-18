using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sharp3D.Math.Core;
using Sharp3D.Math.Geometry3D;

namespace editor
{
    public abstract class EditWorld
    {
        public EditWorld(string name)
        {

        }

        public virtual void initialize(EditorDoc parent)
        {
            mParent = parent;
            Vector3F v = new Vector3F(mWidth / 2.0f, mLength / 2.0f, mHeight / 2.0f);
            mAABB = new AxisAlignedBox(mOrigin - v, mOrigin + v);
        }

        public virtual Plane SeaLevel { get { return mSeaLevel; } }
        public virtual Vector3F OriginPoint { get { return mOrigin; } }
        public virtual Vector3F UpDir { get { return mUpDir; } }
        public virtual float Width { get { return mWidth; } }
        public virtual float Length { get { return mLength; } }
        public virtual float Height { get { return mHeight; } }

        protected EditorDoc mParent = null;

        protected float mWidth  = 4096.0f;   //X
        protected float mLength = 4096.0f;   //Y
        protected float mHeight = 4096.0f;   //Z   

        protected Vector3F mUpDir = Vector3F.ZAxis;
        protected Vector3F mOrigin = Vector3F.Zero;
        protected Plane mSeaLevel = new Plane(Vector3F.ZAxis,Vector3F.Zero);
        protected AxisAlignedBox mAABB;
        
    }
}
