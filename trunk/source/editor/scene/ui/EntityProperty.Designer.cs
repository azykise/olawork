namespace scene
{
    partial class EntityPropertyGrid
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
            this.propertyGridEntity = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // propertyGridEntity
            // 
            this.propertyGridEntity.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridEntity.Location = new System.Drawing.Point(0, 0);
            this.propertyGridEntity.Name = "propertyGridEntity";
            this.propertyGridEntity.Size = new System.Drawing.Size(248, 384);
            this.propertyGridEntity.TabIndex = 0;
            this.propertyGridEntity.ToolbarVisible = false;
            this.propertyGridEntity.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridEntity_PropertyValueChanged);
            // 
            // EntityPropertyGrid
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.propertyGridEntity);
            this.Name = "EntityPropertyGrid";
            this.Size = new System.Drawing.Size(248, 384);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid propertyGridEntity;

    }
}
