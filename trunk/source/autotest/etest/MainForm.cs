using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace etest
{
    using tframework;
    public partial class MainForm : Form
    {
        enum TState
        {
            NONE = 0,
            LOADED,
            TESTING,
            PAUSING,  
        };

        public MainForm()
        {
            InitializeComponent();

            tm = new TestMain();
            tm.MainForm = this;

            this.toolStripMenuItem1.PerformClick();
        }

        TestMain tm = null;
        //string ot_name = "CaseOpenModel";
        string ot_name = "";
        TState mState = TState.NONE;

        private void initTest()
        {
            tm.initialize();
            tm.OnTestFailed += _onTestCaseFailed;
            tm.OnTestPassed += _onTestCasePassed;
            tm.OnOutPutInfo += _onOutPut;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            switch (mState)
            {
                case TState.NONE:
                case TState.PAUSING:
                case TState.LOADED:
                    break;
                case TState.TESTING:
                    this.timer1.Stop();
                    TestMain.EnumCaseResult r = tm.enumerateCases();
                    if (r == TestMain.EnumCaseResult.ENUMCASE_OVER)
                    {
                        tm.shutdown();

                        _onOutPut("  ");
                        _onOutPut("**** Test Over ****");

                        this.button1.Text = "Start";
                        mState = TState.LOADED;  
                    }
                    this.timer1.Start();
                    break;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            switch (mState)
            {
                case TState.LOADED:
                    initTest();
                    foreach (TreeNode rn in this.treeView1.Nodes)
                    {
                        foreach (TreeNode sn in rn.Nodes)
                        {
                            if (sn.Checked)
                            {
                                TestCase ts = sn.Tag as TestCase;
                                if (ts != null)
                                    tm.Cases.Add(ts);
                            }
                        }
                    }
                    _resetTreeNodeColor();
                    this.textBox1.Text = "";
                    _onOutPut("**** Test Start ****");
                    _onOutPut("  ");
                    button1.Text = "Pause";
                    mState = TState.TESTING;
                    break;
                case TState.PAUSING:
                    mState = TState.TESTING;
                    button1.Text = "Pause";
                    break;
                case TState.TESTING:
                    mState = TState.PAUSING;
                    button1.Text = "Start";
                    break;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (mState != TState.NONE)
                return;

            initTest();
            Dictionary<string, List<TestCase>> dic = tm.loadTestCases("./plugins/etest/cases");
            this.treeView1.Nodes.Clear();
            foreach (KeyValuePair<string, List<TestCase>> kvp in dic)
            {
                TreeNode root_node = new TreeNode();
                root_node.Checked = true;
                root_node.Text = kvp.Key;
                root_node.Tag = "root_node";

                foreach (TestCase tcase in kvp.Value)
                {
                    TreeNode node = new TreeNode();
                    node.Checked = true;
                    node.Text = tcase.name();
                    node.Tag = tcase;
                    if (ot_name.Length != 0 && node.Text != ot_name)
                        continue;
                    root_node.Nodes.Add(node);
                }

                if (root_node.Nodes.Count != 0)
                    this.treeView1.Nodes.Add(root_node);
            }
            this.treeView1.ExpandAll();
            button1.Text = "Start";
            mState = TState.LOADED;
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            tm.shutdown();
            button1.Text = "Start";
            textBox1.Text = "";
            mState = this.treeView1.Nodes.Count == 0 ? TState.NONE : TState.LOADED; 
        }

        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Action == TreeViewAction.Unknown)
                return;

            if (e.Node.Tag is string)
            {
                string s = (string)(e.Node.Tag);
                if (s == "root_node")
                {
                    foreach(TreeNode sn in e.Node.Nodes)
                    {
                        sn.Checked = e.Node.Checked;
                    }
                }
            }
        }

        private void _onTestCasePassed(TestCase tcase)
        {
            TreeNode node = _getNodeByTag(tcase);
            if (node != null)
            {                
                node.BackColor = Color.Green;
            }
        }

        private void _onTestCaseFailed(TestCase tcase)
        {
            TreeNode node = _getNodeByTag(tcase);
            if (node != null)
            {
                node.BackColor = Color.Red;
            }
        }

        private void _onOutPut(string s)
        {
            if (s.Trim().Length != 0)
                this.textBox1.AppendText(System.DateTime.Now.ToString("hh_mm_ss") + ": " + s + Environment.NewLine);
            else
                this.textBox1.AppendText(Environment.NewLine);
        }

        private TreeNode _getNodeByTag(object tag)
        {
            foreach (TreeNode rn in this.treeView1.Nodes)
            {
                TreeNode sn = _getNodeByTag(tag, rn);
                if (sn != null)
                {
                    return sn;
                }
            }
            return null;
        }

        private TreeNode _getNodeByTag(object tag, TreeNode parent)
        {
            foreach (TreeNode rn in parent.Nodes)
            {
                if (rn.Tag != null && rn.Tag.Equals(tag))
                    return rn;

                TreeNode sn = _getNodeByTag(tag, rn);
                if (sn != null)
                    return sn;
            }
            return null;
        }

        private void _resetTreeNodeColor()
        {
            foreach (TreeNode rn in this.treeView1.Nodes)
            {
                _resetTreeNodeColor(rn);
            }
        }

        private void _resetTreeNodeColor(TreeNode parent)
        {
            parent.BackColor = Color.Empty;
            foreach (TreeNode rn in parent.Nodes)
            {
                _resetTreeNodeColor(rn);
            }          
        }

        private void treeView1_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            e.Node.Checked = true;
            if (e.Node.Parent != null)
            {
                foreach (TreeNode s_node in e.Node.Parent.Nodes)
                {
                    if (!s_node.Equals(e.Node))
                    {
                        s_node.Checked = !e.Node.Checked;
                    }
                }
            }
        }
    }
}
