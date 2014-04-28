namespace OBJConverter
{
    partial class Form1
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
            this.LoadButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.FileNameTB = new System.Windows.Forms.TextBox();
            this.SaveButton = new System.Windows.Forms.Button();
            this.openFD = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // LoadButton
            // 
            this.LoadButton.Location = new System.Drawing.Point(12, 41);
            this.LoadButton.Name = "LoadButton";
            this.LoadButton.Size = new System.Drawing.Size(128, 23);
            this.LoadButton.TabIndex = 0;
            this.LoadButton.Text = "Open .obj file";
            this.LoadButton.UseVisualStyleBackColor = true;
            this.LoadButton.Click += new System.EventHandler(this.LoadButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Otwarto: ";
            // 
            // FileNameTB
            // 
            this.FileNameTB.Location = new System.Drawing.Point(12, 93);
            this.FileNameTB.Name = "FileNameTB";
            this.FileNameTB.Size = new System.Drawing.Size(100, 20);
            this.FileNameTB.TabIndex = 3;
            this.FileNameTB.Text = "Podaj nazwe pliku";
            // 
            // SaveButton
            // 
            this.SaveButton.Location = new System.Drawing.Point(12, 119);
            this.SaveButton.Name = "SaveButton";
            this.SaveButton.Size = new System.Drawing.Size(116, 23);
            this.SaveButton.TabIndex = 2;
            this.SaveButton.Text = "Zapisz do pliku";
            this.SaveButton.UseVisualStyleBackColor = true;
            this.SaveButton.Click += new System.EventHandler(this.SaveButton_Click);
            // 
            // openFD
            // 
            this.openFD.Filter = "Object|*.obj|All files|*.*";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(525, 206);
            this.Controls.Add(this.FileNameTB);
            this.Controls.Add(this.SaveButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.LoadButton);
            this.Name = "Form1";
            this.Text = ".obj Converter";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button LoadButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox FileNameTB;
        private System.Windows.Forms.Button SaveButton;
        private System.Windows.Forms.OpenFileDialog openFD;
    }
}

