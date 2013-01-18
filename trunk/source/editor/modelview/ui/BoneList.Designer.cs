namespace modelview.ui
{
    partial class BoneList
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonDrawSkl = new System.Windows.Forms.Button();
            this.boneTreeView = new System.Windows.Forms.TreeView();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.textBox1);
            this.panel1.Controls.Add(this.buttonDrawSkl);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 303);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(209, 54);
            this.panel1.TabIndex = 2;
            // 
            // buttonDrawSkl
            // 
            this.buttonDrawSkl.Dock = System.Windows.Forms.DockStyle.Right;
            this.buttonDrawSkl.Location = new System.Drawing.Point(141, 0);
            this.buttonDrawSkl.Name = "buttonDrawSkl";
            this.buttonDrawSkl.Size = new System.Drawing.Size(68, 54);
            this.buttonDrawSkl.TabIndex = 1;
            this.buttonDrawSkl.UseVisualStyleBackColor = true;
            // 
            // boneTreeView
            // 
            this.boneTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.boneTreeView.Location = new System.Drawing.Point(0, 0);
            this.boneTreeView.Name = "boneTreeView";
            this.boneTreeView.Size = new System.Drawing.Size(209, 303);
            this.boneTreeView.TabIndex = 3;
            // 
            // textBox1
            // 
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(141, 54);
            this.textBox1.TabIndex = 2;
            // 
            // BoneList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.boneTreeView);
            this.Controls.Add(this.panel1);
            this.Name = "BoneList";
            this.Size = new System.Drawing.Size(209, 357);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TreeView boneTreeView;
        private System.Windows.Forms.Button buttonDrawSkl;
        private System.Windows.Forms.TextBox textBox1;

    }
}
