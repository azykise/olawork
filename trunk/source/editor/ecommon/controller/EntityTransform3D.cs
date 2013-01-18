using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    using Sharp3D.Math;
    using Sharp3D.Math.Core;

    public abstract class EntityTransform3D
    {
        public virtual List<Entity> TransdEntitys
        {
            get { return mTransdEntitys; }
        }

        public virtual Controller ParentController
        {
            get { return mController; }
            set { mController = value; }
        }

        public virtual void pushToHardwareSelect() { }
        public virtual void render() { }

        protected Vector3F mScale = new Vector3F(1, 1, 1);
        protected List<Entity> mTransdEntitys = null;
        protected Controller mController = null;
    }

    public class EntityDrag3D : EntityTransform3D
    {
        public void onEntityDrag(Vector3F delta_v) { }
    }

    public class EntityMove3D : EntityTransform3D
    {
        public void onEntityMove(Vector3F delta_v) { }
    }

    public class EntityRotate3D : EntityTransform3D
    {
        public void onEntityRotate(Vector3F axis, float radian) { }
    }

    public class EntityScale3D : EntityTransform3D
    {
        public void onEntityScale(Vector3F delta_s) { }
    }

}
