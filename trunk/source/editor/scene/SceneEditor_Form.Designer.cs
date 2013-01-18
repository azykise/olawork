namespace scene
{
    partial class SceneEditor_Form
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.hwRenderPanel1 = new editor.HWRenderPanel();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageCreate = new System.Windows.Forms.TabPage();
            this.tabPageProperty = new System.Windows.Forms.TabPage();
            this.tabPageList = new System.Windows.Forms.TabPage();
            this.tabControl2 = new System.Windows.Forms.TabControl();
            this.tabPageTimeLine = new System.Windows.Forms.TabPage();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabControl2.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripMain
            // 
            this.toolStripMain.AutoSize = false;
            this.toolStripMain.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStripMain.Location = new System.Drawing.Point(0, 0);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(763, 40);
            this.toolStripMain.TabIndex = 0;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 40);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            this.splitContainer1.Panel1MinSize = 0;
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tabControl2);
            this.splitContainer1.Panel2Collapsed = true;
            this.splitContainer1.Panel2MinSize = 0;
            this.splitContainer1.Size = new System.Drawing.Size(763, 385);
            this.splitContainer1.SplitterDistance = 308;
            this.splitContainer1.TabIndex = 1;
            // 
            // splitContainer2
            // 
            this.splitContainer2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.hwRenderPanel1);
            this.splitContainer2.Panel1MinSize = 0;
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.tabControl1);
            this.splitContainer2.Panel2MinSize = 0;
            this.splitContainer2.Size = new System.Drawing.Size(763, 385);
            this.splitContainer2.SplitterDistance = 560;
            this.splitContainer2.TabIndex = 0;
            // 
            // hwRenderPanel1
            // 
            this.hwRenderPanel1.BackColor = System.Drawing.Color.Black;
            this.hwRenderPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.hwRenderPanel1.Location = new System.Drawing.Point(0, 0);
            this.hwRenderPanel1.Name = "hwRenderPanel1";
            this.hwRenderPanel1.Size = new System.Drawing.Size(556, 381);
            this.hwRenderPanel1.TabIndex = 10;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageCreate);
            this.tabControl1.Controls.Add(this.tabPageProperty);
            this.tabControl1.Controls.Add(this.tabPageList);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(195, 381);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageCreate
            // 
            this.tabPageCreate.Location = new System.Drawing.Point(4, 22);
            this.tabPageCreate.Name = "tabPageCreate";
            this.tabPageCreate.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCreate.Size = new System.Drawing.Size(187, 355);
            this.tabPageCreate.TabIndex = 0;
            this.tabPageCreate.Text = "创建";
            this.tabPageCreate.UseVisualStyleBackColor = true;
            // 
            // tabPageProperty
            // 
            this.tabPageProperty.Location = new System.Drawing.Point(4, 22);
            this.tabPageProperty.Name = "tabPageProperty";
            this.tabPageProperty.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageProperty.Size = new System.Drawing.Size(187, 355);
            this.tabPageProperty.TabIndex = 1;
            this.tabPageProperty.Text = "属性";
            this.tabPageProperty.UseVisualStyleBackColor = true;
            // 
            // tabPageList
            // 
            this.tabPageList.Location = new System.Drawing.Point(4, 22);
            this.tabPageList.Name = "tabPageList";
            this.tabPageList.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageList.Size = new System.Drawing.Size(187, 355);
            this.tabPageList.TabIndex = 2;
            this.tabPageList.Text = "列表";
            this.tabPageList.UseVisualStyleBackColor = true;
            // 
            // tabControl2
            // 
            this.tabControl2.Controls.Add(this.tabPageTimeLine);
            this.tabControl2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl2.Location = new System.Drawing.Point(0, 0);
            this.tabControl2.Name = "tabControl2";
            this.tabControl2.SelectedIndex = 0;
            this.tabControl2.Size = new System.Drawing.Size(146, 42);
            this.tabControl2.TabIndex = 0;
            // 
            // tabPageTimeLine
            // 
            this.tabPageTimeLine.Location = new System.Drawing.Point(4, 22);
            this.tabPageTimeLine.Name = "tabPageTimeLine";
            this.tabPageTimeLine.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTimeLine.Size = new System.Drawing.Size(751, 43);
            this.tabPageTimeLine.TabIndex = 0;
            this.tabPageTimeLine.Text = "时间轴";
            this.tabPageTimeLine.UseVisualStyleBackColor = true;
            // 
            // SceneEditor_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.toolStripMain);
            this.Name = "SceneEditor_Form";
            this.Size = new System.Drawing.Size(763, 425);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabControl2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private editor.HWRenderPanel hwRenderPanel1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageCreate;
        private System.Windows.Forms.TabPage tabPageProperty;
        private System.Windows.Forms.TabControl tabControl2;
        private System.Windows.Forms.TabPage tabPageTimeLine;
        private System.Windows.Forms.TabPage tabPageList;

    }
}
