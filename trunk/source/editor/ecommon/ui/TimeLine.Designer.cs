namespace editor.ui
{
    partial class TimeLine
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

        void fun() { }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBoxBack = new editor.ui.TimeLinePictureBox();
            this.buttonControl = new System.Windows.Forms.Button();
            this.buttonMoveLeft = new System.Windows.Forms.Button();
            this.buttonMoveRight = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBack)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxBack
            // 
            this.pictureBoxBack.CurrentFrame = 0;
            this.pictureBoxBack.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxBack.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxBack.Name = "pictureBoxBack";
            this.pictureBoxBack.OffsetX = 0;
            this.pictureBoxBack.ScaleX = 1F;
            this.pictureBoxBack.Size = new System.Drawing.Size(561, 80);
            this.pictureBoxBack.TabIndex = 0;
            this.pictureBoxBack.TabStop = false;
            this.pictureBoxBack.TotalFrame = 100;
            this.pictureBoxBack.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pictureBoxBack_MouseClick);
            // 
            // buttonControl
            // 
            this.buttonControl.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonControl.Location = new System.Drawing.Point(32, 7);
            this.buttonControl.Name = "buttonControl";
            this.buttonControl.Size = new System.Drawing.Size(56, 20);
            this.buttonControl.TabIndex = 1;
            this.buttonControl.Text = "0";
            this.buttonControl.UseVisualStyleBackColor = true;
            // 
            // buttonMoveLeft
            // 
            this.buttonMoveLeft.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonMoveLeft.Location = new System.Drawing.Point(16, 7);
            this.buttonMoveLeft.Name = "buttonMoveLeft";
            this.buttonMoveLeft.Size = new System.Drawing.Size(16, 20);
            this.buttonMoveLeft.TabIndex = 2;
            this.buttonMoveLeft.Text = "<";
            this.buttonMoveLeft.UseVisualStyleBackColor = true;
            // 
            // buttonMoveRight
            // 
            this.buttonMoveRight.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonMoveRight.Location = new System.Drawing.Point(88, 7);
            this.buttonMoveRight.Name = "buttonMoveRight";
            this.buttonMoveRight.Size = new System.Drawing.Size(16, 20);
            this.buttonMoveRight.TabIndex = 3;
            this.buttonMoveRight.Text = ">";
            this.buttonMoveRight.UseVisualStyleBackColor = true;
            // 
            // TimeLine
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.buttonMoveRight);
            this.Controls.Add(this.buttonMoveLeft);
            this.Controls.Add(this.buttonControl);
            this.Controls.Add(this.pictureBoxBack);
            this.Name = "TimeLine";
            this.Size = new System.Drawing.Size(561, 80);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBack)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private TimeLinePictureBox pictureBoxBack;
        private System.Windows.Forms.Button buttonControl;
        private System.Windows.Forms.Button buttonMoveLeft;
        private System.Windows.Forms.Button buttonMoveRight;
    }
}
