namespace modelview.ui
{
    partial class ToolBarCommon
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolBarCommon));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.ButtonResPath = new System.Windows.Forms.ToolStripButton();
            this.ButtonReloadModel = new System.Windows.Forms.ToolStripButton();
            this.ButtonExpandRight = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ButtonResPath,
            this.ButtonReloadModel,
            this.ButtonExpandRight});
            this.toolStrip1.Location = new System.Drawing.Point(35, 27);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(311, 40);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // ButtonResPath
            // 
            this.ButtonResPath.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ButtonResPath.Image = ((System.Drawing.Image)(resources.GetObject("ButtonResPath.Image")));
            this.ButtonResPath.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ButtonResPath.Name = "ButtonResPath";
            this.ButtonResPath.Size = new System.Drawing.Size(36, 37);
            this.ButtonResPath.Text = "toolStripButton1";
            this.ButtonResPath.Click += new System.EventHandler(this.ButtonResPath_Click);
            // 
            // ButtonReloadModel
            // 
            this.ButtonReloadModel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ButtonReloadModel.Image = ((System.Drawing.Image)(resources.GetObject("ButtonReloadModel.Image")));
            this.ButtonReloadModel.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ButtonReloadModel.Name = "ButtonReloadModel";
            this.ButtonReloadModel.Size = new System.Drawing.Size(36, 37);
            this.ButtonReloadModel.Text = "toolStripButton1";
            this.ButtonReloadModel.Click += new System.EventHandler(this.ButtonReloadModel_Click);
            // 
            // ButtonExpandRight
            // 
            this.ButtonExpandRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ButtonExpandRight.Image = ((System.Drawing.Image)(resources.GetObject("ButtonExpandRight.Image")));
            this.ButtonExpandRight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ButtonExpandRight.Name = "ButtonExpandRight";
            this.ButtonExpandRight.Size = new System.Drawing.Size(36, 37);
            this.ButtonExpandRight.Text = "toolStripButton1";
            this.ButtonExpandRight.Click += new System.EventHandler(this.ButtonExpandRight_Click);
            // 
            // ToolBarCommon
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.toolStrip1);
            this.Name = "ToolBarCommon";
            this.Size = new System.Drawing.Size(415, 94);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton ButtonReloadModel;
        private System.Windows.Forms.ToolStripButton ButtonResPath;
        private System.Windows.Forms.ToolStripButton ButtonExpandRight;
    }
}
