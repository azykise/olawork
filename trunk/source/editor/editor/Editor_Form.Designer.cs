namespace editor
{
    partial class Editor_Form
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

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Editor_Form));
            this.m_timer = new System.Windows.Forms.Timer(this.components);
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.buttonItem_file = new System.Windows.Forms.ToolStripButton();
            this.buttonItem_edit = new System.Windows.Forms.ToolStripSplitButton();
            this.buttonItem_log = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonItem_domain = new System.Windows.Forms.ToolStripSplitButton();
            this.panelBack = new System.Windows.Forms.Panel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_timer
            // 
            this.m_timer.Enabled = true;
            this.m_timer.Interval = 1;
            this.m_timer.Tick += new System.EventHandler(this.m_timer_Tick);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonItem_file,
            this.buttonItem_edit,
            this.buttonItem_domain});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.Size = new System.Drawing.Size(906, 25);
            this.toolStrip1.TabIndex = 9;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // buttonItem_file
            // 
            this.buttonItem_file.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.buttonItem_file.Image = ((System.Drawing.Image)(resources.GetObject("buttonItem_file.Image")));
            this.buttonItem_file.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonItem_file.Name = "buttonItem_file";
            this.buttonItem_file.Size = new System.Drawing.Size(36, 22);
            this.buttonItem_file.Text = "文件";
            // 
            // buttonItem_edit
            // 
            this.buttonItem_edit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.buttonItem_edit.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.buttonItem_log});
            this.buttonItem_edit.Image = ((System.Drawing.Image)(resources.GetObject("buttonItem_edit.Image")));
            this.buttonItem_edit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonItem_edit.Name = "buttonItem_edit";
            this.buttonItem_edit.Size = new System.Drawing.Size(48, 22);
            this.buttonItem_edit.Text = "编辑";
            // 
            // buttonItem_log
            // 
            this.buttonItem_log.Name = "buttonItem_log";
            this.buttonItem_log.Size = new System.Drawing.Size(100, 22);
            this.buttonItem_log.Text = "日志";
            this.buttonItem_log.Click += new System.EventHandler(this.buttonItem_log_Click);
            // 
            // buttonItem_domain
            // 
            this.buttonItem_domain.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.buttonItem_domain.Image = ((System.Drawing.Image)(resources.GetObject("buttonItem_domain.Image")));
            this.buttonItem_domain.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.buttonItem_domain.Name = "buttonItem_domain";
            this.buttonItem_domain.Size = new System.Drawing.Size(48, 22);
            this.buttonItem_domain.Text = "启动";
            // 
            // panelBack
            // 
            this.panelBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelBack.Location = new System.Drawing.Point(0, 25);
            this.panelBack.Name = "panelBack";
            this.panelBack.Size = new System.Drawing.Size(906, 594);
            this.panelBack.TabIndex = 10;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 597);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(906, 22);
            this.statusStrip1.TabIndex = 11;
            this.statusStrip1.Text = "statusStrip1";
            this.statusStrip1.Visible = false;
            // 
            // Editor_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(906, 619);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.panelBack);
            this.Controls.Add(this.toolStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Editor_Form";
            this.Text = "未命名";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Editor_Form_FormClosed);
            this.Disposed += new System.EventHandler(this.release);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer m_timer;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.Panel panelBack;
        private System.Windows.Forms.ToolStripButton buttonItem_file;
        private System.Windows.Forms.ToolStripSplitButton buttonItem_domain;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripSplitButton buttonItem_edit;
        private System.Windows.Forms.ToolStripMenuItem buttonItem_log;
    }
}

