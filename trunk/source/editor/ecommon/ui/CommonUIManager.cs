using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace editor.ui
{
    public enum CommonUINote : int
    {
        CameraFocusOn = 2001,

    }

    public abstract class DockTo
    {
        
    }

    public interface IDockableWindow
    {
        void mergeToolBar(System.Windows.Forms.ToolStrip bar);

        void insertDockControl(DockTo dockto, System.Windows.Forms.Control c);
    }

    public class Tool
    {/*
        public static DevComponents.AdvTree.Node GetNodeByText(DevComponents.AdvTree.Node parent,string text)
        {
            if (parent.Text != null && parent.Text == text)
            {
                return parent;
            }
            foreach (DevComponents.AdvTree.Node node in parent.Nodes)
            {
                DevComponents.AdvTree.Node res = GetNodeByText(node, text);
                if (res != null)
                {
                    return res;
                }
            }
            return null;
        }

        public static DevComponents.AdvTree.Node GetNodeByText(DevComponents.AdvTree.AdvTree tree,string text)
        {
            foreach (DevComponents.AdvTree.Node node in tree.Nodes)
            {
                DevComponents.AdvTree.Node res = GetNodeByText(node, text);
                if (res != null)
                {
                    return res;
                }
            }
            return null;
        }*/
    }
}
