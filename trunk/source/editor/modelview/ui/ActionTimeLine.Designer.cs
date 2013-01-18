namespace modelview.ui
{
    partial class ActionTimeLine
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ActionTimeLine));
            this.buttonPause = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolPlayMode = new System.Windows.Forms.ToolStripMenuItem();
            this.toolButtonPlayMode = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonStop = new System.Windows.Forms.Button();
            this.buttonLabel = new System.Windows.Forms.Button();
            this.trackBarAction = new System.Windows.Forms.TrackBar();
            this.panel1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAction)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonPause
            // 
            this.buttonPause.ContextMenuStrip = this.contextMenuStrip1;
            this.buttonPause.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonPause.Enabled = false;
            this.buttonPause.Image = ((System.Drawing.Image)(resources.GetObject("buttonPause.Image")));
            this.buttonPause.Location = new System.Drawing.Point(0, 0);
            this.buttonPause.Name = "buttonPause";
            this.buttonPause.Size = new System.Drawing.Size(60, 55);
            this.buttonPause.TabIndex = 0;
            this.buttonPause.UseVisualStyleBackColor = true;
            this.buttonPause.Visible = false;
            this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.buttonPause);
            this.panel1.Controls.Add(this.buttonPlay);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(516, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(60, 55);
            this.panel1.TabIndex = 1;
            // 
            // buttonPlay
            // 
            this.buttonPlay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonPlay.Image = ((System.Drawing.Image)(resources.GetObject("buttonPlay.Image")));
            this.buttonPlay.Location = new System.Drawing.Point(0, 0);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(60, 55);
            this.buttonPlay.TabIndex = 1;
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolPlayMode});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(149, 26);
            // 
            // toolPlayMode
            // 
            this.toolPlayMode.Name = "toolPlayMode";
            this.toolPlayMode.Size = new System.Drawing.Size(148, 22);
            this.toolPlayMode.Text = "循环播放模式";
            this.toolPlayMode.Click += new System.EventHandler(this.toolPlayMode_Click);
            // 
            // toolButtonPlayMode
            // 
            this.toolButtonPlayMode.Name = "toolButtonPlayMode";
            this.toolButtonPlayMode.Size = new System.Drawing.Size(152, 22);
            this.toolButtonPlayMode.Text = "循环模式";
            // 
            // buttonStop
            // 
            this.buttonStop.ContextMenuStrip = this.contextMenuStrip1;
            this.buttonStop.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonStop.Image = ((System.Drawing.Image)(resources.GetObject("buttonStop.Image")));
            this.buttonStop.Location = new System.Drawing.Point(473, 0);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(43, 55);
            this.buttonStop.TabIndex = 1;
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // buttonLabel
            // 
            this.buttonLabel.ContextMenuStrip = this.contextMenuStrip1;
            this.buttonLabel.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonLabel.Location = new System.Drawing.Point(412, 0);
            this.buttonLabel.Name = "buttonLabel";
            this.buttonLabel.Size = new System.Drawing.Size(61, 55);
            this.buttonLabel.TabIndex = 4;
            this.buttonLabel.Text = "0/100";
            this.buttonLabel.UseVisualStyleBackColor = true;
            // 
            // trackBarAction
            // 
            this.trackBarAction.ContextMenuStrip = this.contextMenuStrip1;
            this.trackBarAction.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBarAction.LargeChange = 1;
            this.trackBarAction.Location = new System.Drawing.Point(0, 0);
            this.trackBarAction.Maximum = 120;
            this.trackBarAction.Name = "trackBarAction";
            this.trackBarAction.Size = new System.Drawing.Size(412, 55);
            this.trackBarAction.TabIndex = 101;
            this.trackBarAction.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarAction.Scroll += new System.EventHandler(this.trackBarAction_Scroll);
            // 
            // ActionTimeLine
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.trackBarAction);
            this.Controls.Add(this.buttonLabel);
            this.Controls.Add(this.buttonStop);
            this.Controls.Add(this.panel1);
            this.Name = "ActionTimeLine";
            this.Size = new System.Drawing.Size(576, 55);
            this.panel1.ResumeLayout(false);
            this.contextMenuStrip1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAction)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonPause;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button buttonPlay;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem toolButtonPlayMode;
        private System.Windows.Forms.ToolStripMenuItem toolPlayMode;
        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Button buttonLabel;
        private System.Windows.Forms.TrackBar trackBarAction;
    }
}
