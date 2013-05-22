namespace modelview.ui
{
    partial class NewMatForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(NewMatForm));
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonPath = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.radioButtonTexture = new System.Windows.Forms.RadioButton();
            this.radioButtonVector4 = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.radioButtonBool = new System.Windows.Forms.RadioButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.radioButtonBool);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.radioButtonVector4);
            this.panel1.Controls.Add(this.radioButtonTexture);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.buttonPath);
            this.panel1.Controls.Add(this.buttonOK);
            this.panel1.Controls.Add(this.textBox1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(284, 209);
            this.panel1.TabIndex = 0;
            // 
            // buttonPath
            // 
            this.buttonPath.Location = new System.Drawing.Point(12, 137);
            this.buttonPath.Name = "buttonPath";
            this.buttonPath.Size = new System.Drawing.Size(259, 23);
            this.buttonPath.TabIndex = 2;
            this.buttonPath.Text = "Path";
            this.buttonPath.UseVisualStyleBackColor = true;
            this.buttonPath.Click += new System.EventHandler(this.buttonPath_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(197, 166);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "确定";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 28);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(259, 21);
            this.textBox1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Name:";
            // 
            // radioButtonTexture
            // 
            this.radioButtonTexture.AutoSize = true;
            this.radioButtonTexture.Location = new System.Drawing.Point(13, 93);
            this.radioButtonTexture.Name = "radioButtonTexture";
            this.radioButtonTexture.Size = new System.Drawing.Size(65, 16);
            this.radioButtonTexture.TabIndex = 4;
            this.radioButtonTexture.Text = "Texture";
            this.radioButtonTexture.UseVisualStyleBackColor = true;
            this.radioButtonTexture.CheckedChanged += new System.EventHandler(this.radioButtonTexture_CheckedChanged);
            // 
            // radioButtonVector4
            // 
            this.radioButtonVector4.AutoSize = true;
            this.radioButtonVector4.Location = new System.Drawing.Point(12, 115);
            this.radioButtonVector4.Name = "radioButtonVector4";
            this.radioButtonVector4.Size = new System.Drawing.Size(65, 16);
            this.radioButtonVector4.TabIndex = 5;
            this.radioButtonVector4.Text = "Vector4";
            this.radioButtonVector4.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "Type:";
            // 
            // radioButtonBool
            // 
            this.radioButtonBool.AutoSize = true;
            this.radioButtonBool.Checked = true;
            this.radioButtonBool.Location = new System.Drawing.Point(14, 71);
            this.radioButtonBool.Name = "radioButtonBool";
            this.radioButtonBool.Size = new System.Drawing.Size(47, 16);
            this.radioButtonBool.TabIndex = 7;
            this.radioButtonBool.TabStop = true;
            this.radioButtonBool.Text = "Bool";
            this.radioButtonBool.UseVisualStyleBackColor = true;
            // 
            // NewMatForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 209);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewMatForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "新建MAT属性";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.NewMatForm_FormClosing);
            this.VisibleChanged += new System.EventHandler(this.NewMatForm_VisibleChanged);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonPath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton radioButtonVector4;
        private System.Windows.Forms.RadioButton radioButtonTexture;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RadioButton radioButtonBool;
    }
}