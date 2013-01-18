using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace modelview.ui
{
    partial class ActionList : UserControl
    {
        public ActionList()
        {
            InitializeComponent();
        }

        public void updateUI()
        {
            mLock = true;
            this.treeViewActions.BeginUpdate();
            this.treeViewActions.Nodes.Clear();

            foreach (KeyValuePair<string,string> kvp in EditorComponent.ActionFiles)
            {
                TreeNode node = new TreeNode();
                node.Name = kvp.Key;
                node.Text = kvp.Key;
                node.Tag = kvp.Value;
                this.treeViewActions.Nodes.Add(node);

                if (EditorComponent.CurrentActionName != null && EditorComponent.CurrentActionName.Length > 0)
                {
                    if(node.Text == EditorComponent.CurrentActionName)
                        this.treeViewActions.SelectedNode = node;
                }
            }

            this.textBox1.Text = EditorComponent.CurrentActionName;

            selectTreenodeByText(EditorComponent.CurrentActionName);

            this.treeViewActions.EndUpdate();
            mLock = false;
        }

        public ActionListComponent EditorComponent = null;
        bool mLock = false;

        private void selectTreenodeByText(string text)
        {
            if (text == null || text.Length == 0)
                return;

            List<TreeNode> nodes = editor.Tool.FindTreeNodesByText(this.treeViewActions, text);
            if (nodes.Count != 0)
            {
                this.treeViewActions.SelectedNode = nodes[0];
            }
        }

        private void treeViewActions_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (mLock)
                return;

            EditorComponent.setCurrentAction(e.Node.Text);
        }

        private void buttonRefreshList_Click(object sender, EventArgs e)
        {
            if (mLock)
                return;

            EditorComponent.refreshActionList();
        }

        private void buttonResetAction_Click(object sender, EventArgs e)
        {
            if (mLock)
                return;

            EditorComponent.resetCurrentAction();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (mLock)
                return;

            string text = textBox1.Text;

            selectTreenodeByText(text);
        }
    }
}
