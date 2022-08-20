namespace DoubleChessOpenerUI
{
    partial class CreateOpeningBookForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.headerLabel = new System.Windows.Forms.Label();
            this.nameLabel = new System.Windows.Forms.Label();
            this.sourceXLabel = new System.Windows.Forms.Label();
            this.sourceYLabel = new System.Windows.Forms.Label();
            this.destXLabel = new System.Windows.Forms.Label();
            this.destYLabel = new System.Windows.Forms.Label();
            this.nameTextBox = new System.Windows.Forms.TextBox();
            this.sourceXTextBox = new System.Windows.Forms.TextBox();
            this.sourceYTextBox = new System.Windows.Forms.TextBox();
            this.destXTextBox = new System.Windows.Forms.TextBox();
            this.destYTextBox = new System.Windows.Forms.TextBox();
            this.plyListBox = new System.Windows.Forms.ListBox();
            this.addPlyButton = new System.Windows.Forms.Button();
            this.createBookButton = new System.Windows.Forms.Button();
            this.removeSelectedButton = new System.Windows.Forms.Button();
            this.board2CheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // headerLabel
            // 
            this.headerLabel.AutoSize = true;
            this.headerLabel.Font = new System.Drawing.Font("Segoe UI", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.headerLabel.ForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.headerLabel.Location = new System.Drawing.Point(167, 9);
            this.headerLabel.Name = "headerLabel";
            this.headerLabel.Size = new System.Drawing.Size(325, 45);
            this.headerLabel.TabIndex = 0;
            this.headerLabel.Text = "Create Opening Book";
            // 
            // nameLabel
            // 
            this.nameLabel.AutoSize = true;
            this.nameLabel.Font = new System.Drawing.Font("Segoe UI", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.nameLabel.Location = new System.Drawing.Point(98, 73);
            this.nameLabel.Name = "nameLabel";
            this.nameLabel.Size = new System.Drawing.Size(66, 25);
            this.nameLabel.TabIndex = 1;
            this.nameLabel.Text = "Name:";
            // 
            // sourceXLabel
            // 
            this.sourceXLabel.AutoSize = true;
            this.sourceXLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.sourceXLabel.Location = new System.Drawing.Point(93, 111);
            this.sourceXLabel.Name = "sourceXLabel";
            this.sourceXLabel.Size = new System.Drawing.Size(71, 21);
            this.sourceXLabel.TabIndex = 2;
            this.sourceXLabel.Text = "Source X";
            // 
            // sourceYLabel
            // 
            this.sourceYLabel.AutoSize = true;
            this.sourceYLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.sourceYLabel.Location = new System.Drawing.Point(198, 111);
            this.sourceYLabel.Name = "sourceYLabel";
            this.sourceYLabel.Size = new System.Drawing.Size(71, 21);
            this.sourceYLabel.TabIndex = 3;
            this.sourceYLabel.Text = "Source Y";
            // 
            // destXLabel
            // 
            this.destXLabel.AutoSize = true;
            this.destXLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.destXLabel.Location = new System.Drawing.Point(93, 167);
            this.destXLabel.Name = "destXLabel";
            this.destXLabel.Size = new System.Drawing.Size(54, 21);
            this.destXLabel.TabIndex = 4;
            this.destXLabel.Text = "Dest X";
            // 
            // destYLabel
            // 
            this.destYLabel.AutoSize = true;
            this.destYLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.destYLabel.Location = new System.Drawing.Point(198, 167);
            this.destYLabel.Name = "destYLabel";
            this.destYLabel.Size = new System.Drawing.Size(54, 21);
            this.destYLabel.TabIndex = 5;
            this.destYLabel.Text = "Dest Y";
            // 
            // nameTextBox
            // 
            this.nameTextBox.Location = new System.Drawing.Point(170, 75);
            this.nameTextBox.Name = "nameTextBox";
            this.nameTextBox.Size = new System.Drawing.Size(322, 23);
            this.nameTextBox.TabIndex = 6;
            // 
            // sourceXTextBox
            // 
            this.sourceXTextBox.Location = new System.Drawing.Point(98, 139);
            this.sourceXTextBox.Name = "sourceXTextBox";
            this.sourceXTextBox.Size = new System.Drawing.Size(56, 23);
            this.sourceXTextBox.TabIndex = 7;
            // 
            // sourceYTextBox
            // 
            this.sourceYTextBox.Location = new System.Drawing.Point(199, 139);
            this.sourceYTextBox.Name = "sourceYTextBox";
            this.sourceYTextBox.Size = new System.Drawing.Size(53, 23);
            this.sourceYTextBox.TabIndex = 8;
            // 
            // destXTextBox
            // 
            this.destXTextBox.Location = new System.Drawing.Point(98, 197);
            this.destXTextBox.Name = "destXTextBox";
            this.destXTextBox.Size = new System.Drawing.Size(56, 23);
            this.destXTextBox.TabIndex = 9;
            // 
            // destYTextBox
            // 
            this.destYTextBox.Location = new System.Drawing.Point(198, 197);
            this.destYTextBox.Name = "destYTextBox";
            this.destYTextBox.Size = new System.Drawing.Size(53, 23);
            this.destYTextBox.TabIndex = 10;
            // 
            // plyListBox
            // 
            this.plyListBox.FormattingEnabled = true;
            this.plyListBox.ItemHeight = 15;
            this.plyListBox.Location = new System.Drawing.Point(423, 111);
            this.plyListBox.Name = "plyListBox";
            this.plyListBox.Size = new System.Drawing.Size(181, 214);
            this.plyListBox.TabIndex = 11;
            // 
            // addPlyButton
            // 
            this.addPlyButton.BackColor = System.Drawing.Color.LightGreen;
            this.addPlyButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.addPlyButton.Font = new System.Drawing.Font("Segoe UI Semibold", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.addPlyButton.Location = new System.Drawing.Point(120, 243);
            this.addPlyButton.Name = "addPlyButton";
            this.addPlyButton.Size = new System.Drawing.Size(108, 55);
            this.addPlyButton.TabIndex = 12;
            this.addPlyButton.Text = "Add Ply";
            this.addPlyButton.UseVisualStyleBackColor = false;
            this.addPlyButton.Click += new System.EventHandler(this.addPlyButton_Click);
            // 
            // createBookButton
            // 
            this.createBookButton.BackColor = System.Drawing.Color.Aquamarine;
            this.createBookButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.createBookButton.Font = new System.Drawing.Font("Segoe UI Semibold", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.createBookButton.Location = new System.Drawing.Point(423, 350);
            this.createBookButton.Name = "createBookButton";
            this.createBookButton.Size = new System.Drawing.Size(143, 75);
            this.createBookButton.TabIndex = 13;
            this.createBookButton.Text = "Create Book";
            this.createBookButton.UseVisualStyleBackColor = false;
            this.createBookButton.Click += new System.EventHandler(this.createBookButton_Click);
            // 
            // removeSelectedButton
            // 
            this.removeSelectedButton.BackColor = System.Drawing.Color.LightGreen;
            this.removeSelectedButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.removeSelectedButton.Font = new System.Drawing.Font("Segoe UI Semibold", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.removeSelectedButton.Location = new System.Drawing.Point(633, 111);
            this.removeSelectedButton.Name = "removeSelectedButton";
            this.removeSelectedButton.Size = new System.Drawing.Size(121, 77);
            this.removeSelectedButton.TabIndex = 14;
            this.removeSelectedButton.Text = "Remove Selected";
            this.removeSelectedButton.UseVisualStyleBackColor = false;
            this.removeSelectedButton.Click += new System.EventHandler(this.removeSelectedButton_Click);
            // 
            // board2CheckBox
            // 
            this.board2CheckBox.AutoSize = true;
            this.board2CheckBox.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.board2CheckBox.Location = new System.Drawing.Point(307, 136);
            this.board2CheckBox.Name = "board2CheckBox";
            this.board2CheckBox.Size = new System.Drawing.Size(83, 25);
            this.board2CheckBox.TabIndex = 15;
            this.board2CheckBox.Text = "Board 2";
            this.board2CheckBox.UseVisualStyleBackColor = true;
            // 
            // CreateOpeningBookForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(766, 450);
            this.Controls.Add(this.board2CheckBox);
            this.Controls.Add(this.removeSelectedButton);
            this.Controls.Add(this.createBookButton);
            this.Controls.Add(this.addPlyButton);
            this.Controls.Add(this.plyListBox);
            this.Controls.Add(this.destYTextBox);
            this.Controls.Add(this.destXTextBox);
            this.Controls.Add(this.sourceYTextBox);
            this.Controls.Add(this.sourceXTextBox);
            this.Controls.Add(this.nameTextBox);
            this.Controls.Add(this.destYLabel);
            this.Controls.Add(this.destXLabel);
            this.Controls.Add(this.sourceYLabel);
            this.Controls.Add(this.sourceXLabel);
            this.Controls.Add(this.nameLabel);
            this.Controls.Add(this.headerLabel);
            this.Name = "CreateOpeningBookForm";
            this.Text = "Create Opening Book";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Label headerLabel;
        private Label nameLabel;
        private Label sourceXLabel;
        private Label sourceYLabel;
        private Label destXLabel;
        private Label destYLabel;
        private TextBox nameTextBox;
        private TextBox sourceXTextBox;
        private TextBox sourceYTextBox;
        private TextBox destXTextBox;
        private TextBox destYTextBox;
        private ListBox plyListBox;
        private Button addPlyButton;
        private Button createBookButton;
        private Button removeSelectedButton;
        private CheckBox board2CheckBox;
    }
}