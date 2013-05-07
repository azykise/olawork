using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public class EntityBuffer : IEnumerable<Entity>
    {
        public EntityBuffer()
        {
            
        }

        public EntityBuffer(EntityBuffer buffer)
        {
            mParent = buffer.Parent;
            mHandler = buffer.Handler;

            merge(buffer); 
        }

        IEnumerator<Entity> IEnumerable<Entity>.GetEnumerator()
        {
            return mEntitys.Values.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return mEntitys.GetEnumerator();
        }

        public virtual int merge(EntityBuffer buffer)
        {
            int count = 0;
            foreach (Entity entity in buffer)
            {
                count = insert(entity) ? count + 1 : count;
            }
            return count;
        }

        public virtual bool insert(Entity entity)
        {
            if (!mEntitys.ContainsKey(entity.ID))
            {
                mEntitys.Add(entity.ID, entity);
                Type type = entity.GetType();
                if (!mCacheBasedonType.ContainsKey(type))
                {
                    mCacheBasedonType[type] = new List<Entity>();
                }
                mCacheBasedonType[type].Add(entity);

                if (mParent != null)
                {
                    entity.Layer = mParent;
                }

                return true;
            }
            else
                return false;
        }

        public virtual bool remove(Entity entity)
        {
            if (mEntitys.ContainsKey(entity.ID))
            {
                mEntitys.Remove(entity.ID);
                mCacheBasedonType[entity.GetType()].Remove(entity);
                entity.Layer = null;
                return true;
            }
            else
                return false;
        }

        public virtual void clear()
        {
            mEntitys.Clear();
            mCacheBasedonType.Clear();
        }

        public virtual Entity find(UInt16 id)
        {
            return mEntitys.ContainsKey(id) ? mEntitys[id] : null;
        }

        public virtual List<Entity> getOnType(Type t)
        {
            List<Entity> entitys = new List<Entity>();
            if (mCacheBasedonType.ContainsKey(t))
            {
                entitys.AddRange(mCacheBasedonType[t]);
            }
            return entitys;
        }

        public virtual Entity first()
        {
            foreach (Entity entity in mEntitys.Values)
            {
                return entity;
            }
            return null;
        }
        
        public int count { get { return mEntitys.Count; } }
        public EntityLayer Parent { get { return mParent; } set { mParent = value; } }
        public EntityBufferHandler Handler { get { return mHandler; } set { mHandler = value; } }

        protected EntityLayer mParent = null;
        protected Dictionary<UInt64, Entity> mEntitys = new Dictionary<UInt64, Entity>();
        protected Dictionary<Type, List<Entity>> mCacheBasedonType = new Dictionary<Type, List<Entity>>();
        protected EntityBufferHandler mHandler = null;
        
        public class EntityBufferHandler
        {   //将actor添加和删除的管理类 可能在添加和删除actor的时候需要有很多额外的操作 暂时不考虑
            public EntityBufferHandler(EntityBuffer buffer, EntityManager mng) { mBuffer = buffer; mManager = mng; }
            public void insertEntity(Entity entity){}
            public void removeEntity(Entity entity){}
            public void clearEntity(){}
            EntityBuffer mBuffer = null;
            EntityManager mManager = null;
        }
    }

    public class EntitySelectBuffer : EntityBuffer
    {
        public override bool insert(Entity entity)
        {
            bool r = base.insert(entity);
            if (r)
                entity.Selected = true;
            return r;
        }

        public override bool remove(Entity entity)
        {
            bool r = base.remove(entity);
            if (r)
                entity.Selected = false;
            return r;
        }

        public override void clear()
        {
            foreach (Entity entity in mEntitys.Values)
            {
                entity.Selected = false;
            }
            base.clear();
        }
    }

    public class EntityLayer
    {
        public EntityLayer(string name,EntityManager mng)
        {
            mParent = mng;
            mOriginBuffer.Parent = this;
            mSelectBuffer.Parent = this;
        }

        public EntityBuffer SelectBuffer { get { return mSelectBuffer; } }
        public EntityBuffer OriginBuffer { get { return mOriginBuffer; } }

        public string Name { get { return mName; } }
        public bool Visible { get { return mVisible; } }
        public EntityManager Parent { get { return mParent; } }

        protected EntitySelectBuffer mSelectBuffer = new EntitySelectBuffer();
        protected EntityBuffer mOriginBuffer = new EntityBuffer();

        protected string mName = "untitle";
        protected bool mVisible = true;
        protected EntityManager mParent = null;
        protected Dictionary<uint, Entity> mSelectCache = new Dictionary<uint, Entity>();
    }

    public interface IEntityFactory
    {
        Entity createEntity(string type, Dictionary<string, object> propertys, RenderSystem rendersys);
        List<string> getSupportTypes();
    }

    //实体管理器 不同的编辑器可能有不同的实体管理方式
    public abstract class EntityManager : EditorComponent
    {
        public EntityManager(string name)
            :base(name)
        {

        }

        public virtual void initialize()
        {
            mEntityLayers.Clear();
            this.CurrentEntityLayer = new EntityLayer("layer0", this);
        }

        public virtual void update(float elaspedTime)
        {
            foreach (EntityLayer layer in mEntityLayers.Values)
            {
                foreach (Entity entity in layer.OriginBuffer)
                {
                    entity.update(elaspedTime);
                }
            }
        }

        public virtual void render( RenderSystem rendersys)
        {
            foreach (EntityLayer layer in mEntityLayers.Values)
            {
                if (!layer.Visible)
                    continue;

                foreach (Entity entity in layer.OriginBuffer)
                {
                    entity.render(rendersys);
                }
            }
        }

        public virtual EntityLayer getEntityLayer(string name)
        {
            return mEntityLayers.ContainsKey(name) ? mEntityLayers[name] : null;
        }

        public virtual List<EntityLayer> getEntityLayers()
        {
            List<EntityLayer> layers = new List<EntityLayer>();
            foreach (EntityLayer layer in mEntityLayers.Values)
            {
                layers.Add(layer);
            }
            return layers;
        }

        public virtual List<Entity> quickGetSelectedEntitys()
        {
            List<Entity> l = new List<Entity>();
            foreach (EntityLayer layer in mEntityLayers.Values)
            {
                foreach (Entity sel in layer.SelectBuffer)
                {
                    l.Add(sel);
                }
            }
            return l;
        }

        public virtual EntityLayer CurrentEntityLayer
        {
            get { return mCurrentEntityLayer; }
            set
            {
                mCurrentEntityLayer = value;
                if (mCurrentEntityLayer != null)
                {
                    string name = mCurrentEntityLayer.Name;
                    mEntityLayers[name] = mCurrentEntityLayer;
                }
            }
        }

        //当前的物品层 所有的物品层
        protected EntityLayer mCurrentEntityLayer = null;
        protected Dictionary<string, EntityLayer> mEntityLayers = new Dictionary<string, EntityLayer>();
    }

    public enum CommonEntityNode : int
    {
        EntityCreated = 988,
        SelectedEntityChanged, 
    }
}
