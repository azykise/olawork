using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public abstract class EditorDoc : EditorComponent
    {
        //文档构建的时候必须有个名字,作为Component名称,不能重复
        //filename作为文档标识名称
        //多文档支持需要每个文档下面都有独立的编辑组件故暂时不支持,只用单文档
        public EditorDoc(string name)
            : base(name)
        {
        
        }

        public virtual void newDoc()
        {
            m_filename = "Untitled";
            initialize();
        }

        protected abstract void initialize();

        public virtual bool open(string filename)
        {
            return true;
        }

        public virtual void close()
        {

        }

        public virtual bool save(string filename)
        {
            return true;
        }

        public virtual bool saveAs(string filename)
        {
            return true;
        }

        public virtual void release()
        {

        }

        public override NotifyResult notified(EditorComponent from, MessageBase msg)
        {
            return NotifyResult.Succeed;
        }

        public string Filename 
        {
            get { return m_filename; }
            set { m_filename = value; }
        }

        public EntityManager EntityMng
        {
            get { return mEntityManager; }
            set { mEntityManager = value; }
        }

        public virtual EditWorld World { get { return null; } }
            
        protected string m_filename = "Untitled";
        protected EntityManager mEntityManager = null;
    }
}
