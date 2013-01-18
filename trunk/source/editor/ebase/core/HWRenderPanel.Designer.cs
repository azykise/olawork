namespace editor
{
    partial class HWRenderPanel
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
            this.SuspendLayout();
            // 
            // HWRenderPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Black;
            this.Name = "HWRenderPanel";
            this.MouseLeave += new System.EventHandler(this.HWRenderPanel_MouseLeave);
            this.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.HWRenderPanel_MouseWheel);
            this.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.HWRenderPanel_PreviewKeyDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.HWRenderPanel_MouseMove);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.HWRenderPanel_KeyUp);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.HWRenderPanel_MouseDown);
            this.Resize += new System.EventHandler(this.HWRenderPanel_Resize);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.HWRenderPanel_KeyPress);
            this.MouseHover += new System.EventHandler(this.HWRenderPanel_MouseHover);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.HWRenderPanel_MouseUp);
            this.MouseEnter += new System.EventHandler(this.HWRenderPanel_MouseEnter);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.HWRenderPanel_KeyDown);
            this.ResumeLayout(false);

        }

        #endregion
    }
}
