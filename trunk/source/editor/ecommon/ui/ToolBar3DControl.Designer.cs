namespace editor.ui
{
    partial class ToolBar3DControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolBar3DControl));
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.buttonSelect = new System.Windows.Forms.ToolStripButton();
            this.buttonMove = new System.Windows.Forms.ToolStripButton();
            this.buttonRotate = new System.Windows.Forms.ToolStripButton();
            this.buttonScale = new System.Windows.Forms.ToolStripButton();
            this.buttonGrid = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStrip1
            // 
            this.toolStrip1.AutoSize = false;
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonSelect,
            this.buttonMove,
            this.buttonRotate,
            this.buttonScale,
            this.buttonGrid});
            this.toolStrip1.Location = new System.Drawing.Point(41, 20);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(311, 40);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // buttonSelect
            // 
            this.buttonSelect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonSelect.Image = ((System.Drawing.Image)(resources.GetObject("buttonSelect.Image")));
            this.buttonSelect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonSelect.Name = "buttonSelect";
            this.buttonSelect.Size = new System.Drawing.Size(36, 37);
            this.buttonSelect.Text = "toolStripButton2";
            this.buttonSelect.Click += new System.EventHandler(this.buttonSelect_Click);
            // 
            // buttonMove
            // 
            this.buttonMove.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonMove.Image = ((System.Drawing.Image)(resources.GetObject("buttonMove.Image")));
            this.buttonMove.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonMove.Name = "buttonMove";
            this.buttonMove.Size = new System.Drawing.Size(36, 37);
            this.buttonMove.Text = "toolStripButton1";
            this.buttonMove.Click += new System.EventHandler(this.buttonMove_Click);
            // 
            // buttonRotate
            // 
            this.buttonRotate.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonRotate.Image = ((System.Drawing.Image)(resources.GetObject("buttonRotate.Image")));
            this.buttonRotate.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonRotate.Name = "buttonRotate";
            this.buttonRotate.Size = new System.Drawing.Size(36, 37);
            this.buttonRotate.Text = "toolStripButton3";
            this.buttonRotate.Click += new System.EventHandler(this.buttonRotate_Click);
            // 
            // buttonScale
            // 
            this.buttonScale.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonScale.Image = ((System.Drawing.Image)(resources.GetObject("buttonScale.Image")));
            this.buttonScale.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonScale.Name = "buttonScale";
            this.buttonScale.Size = new System.Drawing.Size(36, 37);
            this.buttonScale.Text = "toolStripButton4";
            this.buttonScale.Click += new System.EventHandler(this.buttonScale_Click);
            // 
            // buttonGrid
            // 
            this.buttonGrid.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.buttonGrid.Image = ((System.Drawing.Image)(resources.GetObject("buttonGrid.Image")));
            this.buttonGrid.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonGrid.Name = "buttonGrid";
            this.buttonGrid.Size = new System.Drawing.Size(36, 37);
            this.buttonGrid.Text = "toolStripButton1";
            this.buttonGrid.Click += new System.EventHandler(this.buttonGrid_Click);
            // 
            // ToolBar3DControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.toolStrip1);
            this.Name = "ToolBar3DControl";
            this.Size = new System.Drawing.Size(373, 86);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton buttonSelect;
        private System.Windows.Forms.ToolStripButton buttonMove;
        private System.Windows.Forms.ToolStripButton buttonRotate;
        private System.Windows.Forms.ToolStripButton buttonScale;
        private System.Windows.Forms.ToolStripButton buttonGrid;

    }
}
