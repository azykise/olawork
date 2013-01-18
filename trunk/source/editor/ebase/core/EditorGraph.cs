using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor
{
    public enum NotifyResult
    {
        Pending,
        Succeed,
        Failed,
    }

    public enum MessageType : int
    {
        Unknown = -1,

        Log,

        Doc,
        NewDoc,

        Editor,

        End,
    }

    public class MessageBase
    {
        public MessageBase(string id)
        {
            mGuid = id;
        }
        public MessageBase(string id,string info0)
        {
            mGuid = id;
            if (info0.Length > 0)
                m_information.Add(info0);
        }
        public MessageBase(MessageType type,System.Enum note)
        {
            m_type = type;
            m_note = note;
        }

        public MessageBase(MessageType type, System.Enum note, string info0)
        {
            m_type = type;
            m_note = note;
            if (info0.Length > 0)
                m_information.Add(info0);
        }

        public MessageBase(MessageType type,string info0)
        {
            m_type = type;
            if(info0.Length > 0)
                m_information.Add(info0);
        }

        public MessageBase(MessageType type, string info0, string info1)
        {
            m_type = type;
            if(info0.Length > 0)
                m_information.Add(info0);

            if (info1.Length > 0)
                m_information.Add(info1);
        }

        public virtual string guid() { return mGuid; }

        public virtual bool contain(string info)
        {
            return m_information.Contains(info);
        }

        public virtual string info(int idx)
        {
            if (idx >= 0 && idx < m_information.Count)
                return m_information[idx];
            else
                return "";        
        }

        public T getNote<T>()
        {
            return m_note == null ? default(T) : toTypedEnum<T>(m_note);
        }

        public static T toTypedEnum<T>(System.Enum e)
        {          
            System.Type t_from = e.GetType();
            System.Type t_to = typeof(T);

            if (t_from == t_to)
            {
                return (T)((Object)e);
            }
            else
                return default(T);
        }

        public virtual List<string> Messages { get { return m_information; } }
        public virtual MessageType MsgType { get { return m_type; } }
        protected List<string> m_information = new List<string>();
        protected MessageType m_type = MessageType.Unknown;
        protected System.Enum m_note = null;
        protected string mGuid = "";
    }

    public abstract class EditorComponent
    {
        public EditorComponent(string name) { m_name = name; }
        public virtual string getName(){ return m_name; }
        public virtual NotifyResult notified(EditorComponent from, MessageBase msg) { return NotifyResult.Failed; }

        public delegate void OnMessage(EditorComponent from,MessageBase msg);

        public void registMessage(string msg_guid, OnMessage handler)
        {
            if (msg_guid.Length == 0)
                return;

            if (!EditorGraph.MessageListenersTable.ContainsKey(msg_guid))
                EditorGraph.MessageListenersTable[msg_guid] = new List<EditorComponent>();

            EditorGraph.MessageListenersTable[msg_guid].Add(this);
            
            if (!EditorGraph.MessageHandlerTable.ContainsKey(msg_guid))
            {
                EditorGraph.MessageHandlerTable[msg_guid] = new List<EditorComponent.OnMessage>();
            }

            EditorGraph.MessageHandlerTable[msg_guid].Add(handler);
        }

        protected string m_name = "";
    }

    class DummyComponent : EditorComponent //虚拟编辑组件,主要给rootnode用
    {
        public DummyComponent(string name):base(name) {}
        public override NotifyResult notified(EditorComponent from, MessageBase msg) { return NotifyResult.Succeed; }
    }

    public class GraphNode
    {
        public GraphNode(GraphNode parent, EditorComponent data)
        {
            m_parent = parent;
            m_data = data;
        }

        public EditorComponent Data { get { return m_data; } }
        public GraphNode Parent { get { return m_parent; } }
        public List<GraphNode> Child { get { return m_child; } }

        EditorComponent m_data = null;
        GraphNode m_parent = null;
        List<GraphNode> m_child = new List<GraphNode>();
    }

    public class EditorGraph
    {
        public EditorGraph()
        {

        }

        public static void init()
        {
            //把rootnode也注册到cache中
            RootNode = new GraphNode(null, new DummyComponent("RootComponent"));

            EditorComponents.Clear();
            ComponentNodeCache.Clear();

            string root_name = RootNode.Data.getName();
            EditorComponents[root_name] = RootNode.Data;
            ComponentNodeCache[root_name] = RootNode;
        }

        public static void release()
        {
            RootNode = null;
            EditorComponents.Clear();
            ComponentNodeCache.Clear();
            MessageHandlerTable.Clear();
            MessageListenersTable.Clear();
        }

        public static NotifyResult notify(EditorComponent to, EditorComponent from, MessageBase msg)
        {
            return to.notified(from, msg);
        }
        public static NotifyResult notify(string name, EditorComponent from, MessageBase msg)
        {
            EditorComponent to = getComponent<EditorComponent>(name);
            if (to != null)
                return to.notified(from, msg);
            else
                return NotifyResult.Failed;
        }
        //fixme 处理交叉发送消息
        public static NotifyResult broadCast(EditorComponent to, EditorComponent from, MessageBase msg)
        {
            string dest_name = to.getName();
            if (ComponentNodeCache.ContainsKey(dest_name))
            {
                GraphNode node = ComponentNodeCache[dest_name];
                foreach(GraphNode sub_node in node.Child)
                {
                    broadCast(sub_node.Data, from, msg);
                }
                return to.notified(from, msg);
            }
            else
                return NotifyResult.Failed;
        }

        public static NotifyResult broadCast(EditorComponent from, MessageBase msg)
        {
            foreach (GraphNode sub_node in EditorGraph.RootNode.Child)
            {
                broadCast(sub_node.Data, from, msg);
            }
            return NotifyResult.Succeed;
        }

        public static NotifyResult postMessage(EditorComponent from, MessageBase msg)
        {
            string msg_guid = msg.guid();
            if (msg_guid.Length == 0)
                return NotifyResult.Failed;

            if (MessageListenersTable.ContainsKey(msg_guid))
            {
                List<EditorComponent> comps = MessageListenersTable[msg_guid];
                foreach (EditorComponent comp in comps)
                {
                    comp.notified(from, msg);
                }
            }

            if (MessageHandlerTable.ContainsKey(msg_guid))
            {
                List<EditorComponent.OnMessage> handles = MessageHandlerTable[msg_guid];
                foreach (EditorComponent.OnMessage handle in handles)
                {
                    handle(from, msg);
                }
            }

            return NotifyResult.Succeed;
        }

        public static void registerComponent(EditorComponent parent, EditorComponent comp)
        {
            GraphNode parent_node = parent != null ? findNode(parent) : null;
            if (parent_node == null)
                parent_node = RootNode;

            GraphNode new_node = new GraphNode(parent_node, comp);
            parent_node.Child.Add(new_node);

            string comp_name = comp.getName();

            if (EditorComponents.ContainsKey(comp_name) || ComponentNodeCache.ContainsKey(comp_name))
                throw new Exception("editor component name conflict!");

            EditorComponents[comp_name] = comp;
            ComponentNodeCache[comp_name] = new_node;
        }

        public static T getComponent<T>(string name)
        {
            if (EditorComponents.ContainsKey(name))
            {                
                return (T)((object)(EditorComponents[name]));
            }
            else
                return default(T);
        }

        public static List<EditorComponent> getParents(EditorComponent comp)
        {
            GraphNode node = findNode(comp);
            List<EditorComponent> parents = new List<EditorComponent>();

            GraphNode parent_node = node.Parent;
            while (parent_node != null)
            {
                EditorComponent parent = parent_node.Data;
                if (parent != null)
                {
                    parents.Add(parent);
                }
                parent_node = parent_node.Parent;
            }
            return parents;
        }

        private static GraphNode findNode(EditorComponent comp)
        {
            if (ComponentNodeCache.ContainsKey(comp.getName()))
                return ComponentNodeCache[comp.getName()];
            else
                return null;
        }

        private static GraphNode findNode(GraphNode node, EditorComponent comp)
        {
            if (node.Data != null && node.Data.Equals(comp))
                return node;
            else if (node.Child.Count > 0)
            {
                GraphNode res = null;
                foreach(GraphNode sub_node in node.Child)
                {
                    res = findNode(sub_node, comp);
                    if (res != null)
                        break;
                }
                return res;
            }
            else
                return null;
        }
        
        public static GraphNode RootNode = new GraphNode(null,new DummyComponent("RootComponent"));
        public static Dictionary<string, EditorComponent> EditorComponents = new Dictionary<string, EditorComponent>();
        public static Dictionary<string, GraphNode> ComponentNodeCache = new Dictionary<string, GraphNode>();

        public static Dictionary<string, List<EditorComponent>> MessageListenersTable = new Dictionary<string, List<EditorComponent>>();
        public static Dictionary<string, List<EditorComponent.OnMessage>> MessageHandlerTable = new Dictionary<string, List<EditorComponent.OnMessage>>();
    }
}
