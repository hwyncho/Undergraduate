namespace MyPacketClient
{
    partial class MyClient
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.labelPort = new System.Windows.Forms.Label();
            this.textBoxIP = new System.Windows.Forms.TextBox();
            this.labelIP = new System.Windows.Forms.Label();
            this.buttonPath = new System.Windows.Forms.Button();
            this.buttonSend = new System.Windows.Forms.Button();
            this.buttonFile = new System.Windows.Forms.Button();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.labelPath = new System.Windows.Forms.Label();
            this.textBoxPath = new System.Windows.Forms.TextBox();
            this.textBoxFile = new System.Windows.Forms.TextBox();
            this.labelFile = new System.Windows.Forms.Label();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.listView = new System.Windows.Forms.ListView();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(406, 15);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(134, 35);
            this.textBoxPort.TabIndex = 2;
            // 
            // labelPort
            // 
            this.labelPort.AutoSize = true;
            this.labelPort.Location = new System.Drawing.Point(330, 20);
            this.labelPort.Name = "labelPort";
            this.labelPort.Size = new System.Drawing.Size(74, 24);
            this.labelPort.TabIndex = 0;
            this.labelPort.Text = "포트 :";
            // 
            // textBoxIP
            // 
            this.textBoxIP.Location = new System.Drawing.Point(83, 15);
            this.textBoxIP.Name = "textBoxIP";
            this.textBoxIP.Size = new System.Drawing.Size(228, 35);
            this.textBoxIP.TabIndex = 1;
            // 
            // labelIP
            // 
            this.labelIP.AutoSize = true;
            this.labelIP.Location = new System.Drawing.Point(30, 20);
            this.labelIP.Name = "labelIP";
            this.labelIP.Size = new System.Drawing.Size(45, 24);
            this.labelIP.TabIndex = 0;
            this.labelIP.Text = "IP :";
            // 
            // buttonPath
            // 
            this.buttonPath.Location = new System.Drawing.Point(30, 84);
            this.buttonPath.Name = "buttonPath";
            this.buttonPath.Size = new System.Drawing.Size(162, 67);
            this.buttonPath.TabIndex = 3;
            this.buttonPath.Text = "다운로드\r\n위치";
            this.buttonPath.UseVisualStyleBackColor = true;
            this.buttonPath.Click += new System.EventHandler(this.buttonPath_Click);
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(378, 84);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(162, 67);
            this.buttonSend.TabIndex = 5;
            this.buttonSend.Text = "업로드";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // buttonFile
            // 
            this.buttonFile.Location = new System.Drawing.Point(205, 84);
            this.buttonFile.Name = "buttonFile";
            this.buttonFile.Size = new System.Drawing.Size(162, 67);
            this.buttonFile.TabIndex = 4;
            this.buttonFile.Text = "보낼 파일";
            this.buttonFile.UseVisualStyleBackColor = true;
            this.buttonFile.Click += new System.EventHandler(this.buttonFile_Click);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(30, 167);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(510, 67);
            this.buttonConnect.TabIndex = 6;
            this.buttonConnect.Text = "접속";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // labelPath
            // 
            this.labelPath.AutoSize = true;
            this.labelPath.Location = new System.Drawing.Point(32, 254);
            this.labelPath.Name = "labelPath";
            this.labelPath.Size = new System.Drawing.Size(74, 24);
            this.labelPath.TabIndex = 0;
            this.labelPath.Text = "저장 :";
            // 
            // textBoxPath
            // 
            this.textBoxPath.Location = new System.Drawing.Point(112, 249);
            this.textBoxPath.Name = "textBoxPath";
            this.textBoxPath.ReadOnly = true;
            this.textBoxPath.Size = new System.Drawing.Size(428, 35);
            this.textBoxPath.TabIndex = 8;
            // 
            // textBoxFile
            // 
            this.textBoxFile.Location = new System.Drawing.Point(112, 293);
            this.textBoxFile.Name = "textBoxFile";
            this.textBoxFile.ReadOnly = true;
            this.textBoxFile.Size = new System.Drawing.Size(428, 35);
            this.textBoxFile.TabIndex = 10;
            // 
            // labelFile
            // 
            this.labelFile.AutoSize = true;
            this.labelFile.Location = new System.Drawing.Point(32, 298);
            this.labelFile.Name = "labelFile";
            this.labelFile.Size = new System.Drawing.Size(74, 24);
            this.labelFile.TabIndex = 0;
            this.labelFile.Text = "파일 :";
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(32, 342);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(508, 48);
            this.progressBar.TabIndex = 0;
            // 
            // listView
            // 
            this.listView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.listView.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.listView.FullRowSelect = true;
            this.listView.Location = new System.Drawing.Point(0, 414);
            this.listView.Name = "listView";
            this.listView.Size = new System.Drawing.Size(570, 411);
            this.listView.TabIndex = 0;
            this.listView.UseCompatibleStateImageBehavior = false;
            this.listView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView_MouseDoubleClick);
            // 
            // Client
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(570, 825);
            this.Controls.Add(this.listView);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.textBoxFile);
            this.Controls.Add(this.labelFile);
            this.Controls.Add(this.textBoxPath);
            this.Controls.Add(this.labelPath);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.buttonFile);
            this.Controls.Add(this.buttonSend);
            this.Controls.Add(this.buttonPath);
            this.Controls.Add(this.textBoxPort);
            this.Controls.Add(this.labelPort);
            this.Controls.Add(this.textBoxIP);
            this.Controls.Add(this.labelIP);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "Client";
            this.Text = "Client";
            this.Load += new System.EventHandler(this.Client_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label labelPort;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Label labelIP;
        private System.Windows.Forms.Button buttonPath;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.Button buttonFile;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Label labelPath;
        private System.Windows.Forms.TextBox textBoxPath;
        private System.Windows.Forms.TextBox textBoxFile;
        private System.Windows.Forms.Label labelFile;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.ListView listView;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
    }
}

