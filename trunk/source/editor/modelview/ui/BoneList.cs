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
    partial class BoneList : UserControl
    {
        public BoneList()
        {
            InitializeComponent();
        }

        int mBoneNumber = 0;

        public void updateBoneList(Engine.Bone root_bone)
        {
            this.boneTreeView.Nodes.Clear();

            if (root_bone == null)
            {
                mBoneNumber = 0;
            }
            else
            {
                mBoneNumber = 1;

                TreeNode root_node = new TreeNode();
                root_node.Text = root_bone.Name;

                this.boneTreeView.Nodes.Add(root_node);

                _insertBoneNode(root_node, root_bone);                

                this.boneTreeView.ExpandAll();
            }

            this.textBox1.Text = "骨骼数目: " + mBoneNumber.ToString();
        }

        private void _insertBoneNode(TreeNode parent_node,Engine.Bone bone)
        {
            foreach (Engine.Bone sub_bone in bone.Children)
            {
                mBoneNumber += 1;
                TreeNode sub_node = new TreeNode();
                sub_node.Text = sub_bone.Name;
                parent_node.Nodes.Add(sub_node);

                _insertBoneNode(sub_node, sub_bone);
            }
        }

        public BoneListComponent EditorComponent = null;
    }
}
