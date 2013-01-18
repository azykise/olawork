namespace editor.ui
{
    partial class ToolBarDocControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolBarDocControl));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.buttonNewDoc = new System.Windows.Forms.ToolStripButton();
            this.buttonOpenDoc = new System.Windows.Forms.ToolStripButton();
            this.buttonDocSave = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonNewDoc,
            this.buttonOpenDoc,
            this.buttonDocSave});
            this.toolStrip1.Location = new System.Drawing.Point(23, 22);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(207, 40);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // buttonNewDoc
            // 
            this.buttonNewDoc.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonNewDoc.Image = ((System.Drawing.Image)(resources.GetObject("buttonNewDoc.Image")));
            this.buttonNewDoc.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonNewDoc.Name = "buttonNewDoc";
            this.buttonNewDoc.Size = new System.Drawing.Size(36, 37);
            this.buttonNewDoc.Text = "toolStripButton1";
            this.buttonNewDoc.Click += new System.EventHandler(this.buttonNewDoc_Click);
            // 
            // buttonOpenDoc
            // 
            this.buttonOpenDoc.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonOpenDoc.Image = ((System.Drawing.Image)(resources.GetObject("buttonOpenDoc.Image")));
            this.buttonOpenDoc.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonOpenDoc.Name = "buttonOpenDoc";
            this.buttonOpenDoc.Size = new System.Drawing.Size(36, 37);
            this.buttonOpenDoc.Text = "toolStripButton2";
            this.buttonOpenDoc.Click += new System.EventHandler(this.buttonOpenDoc_Click);
            // 
            // buttonDocSave
            // 
            this.buttonDocSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonDocSave.Image = ((System.Drawing.Image)(resources.GetObject("buttonDocSave.Image")));
            this.buttonDocSave.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonDocSave.Name = "buttonDocSave";
            this.buttonDocSave.Size = new System.Drawing.Size(36, 37);
            this.buttonDocSave.Text = "toolStripButton3";
            this.buttonDocSave.Click += new System.EventHandler(this.buttonDocSave_Click);
            // 
            // ToolBarDocControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.toolStrip1);
            this.Name = "ToolBarDocControl";
            this.Size = new System.Drawing.Size(287, 88);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton buttonNewDoc;
        private System.Windows.Forms.ToolStripButton buttonOpenDoc;
        private System.Windows.Forms.ToolStripButton buttonDocSave;

    }
}
