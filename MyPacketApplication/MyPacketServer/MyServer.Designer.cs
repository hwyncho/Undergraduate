namespace MyPacketServer
{
    partial class MyServer
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
            this.labelIP = new System.Windows.Forms.Label();
            this.textBoxIP = new System.Windows.Forms.TextBox();
            this.labelPort = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.labelPath = new System.Windows.Forms.Label();
            this.textBoxPath = new System.Windows.Forms.TextBox();
            this.buttonPath = new System.Windows.Forms.Button();
            this.buttonStartStop = new System.Windows.Forms.Button();
            this.textBoxState = new System.Windows.Forms.TextBox();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.SuspendLayout();
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
            // textBoxIP
            // 
            this.textBoxIP.Location = new System.Drawing.Point(81, 15);
            this.textBoxIP.Name = "textBoxIP";
            this.textBoxIP.ReadOnly = true;
            this.textBoxIP.Size = new System.Drawing.Size(477, 35);
            this.textBoxIP.TabIndex = 4;
            // 
            // labelPort
            // 
            this.labelPort.AutoSize = true;
            this.labelPort.Location = new System.Drawing.Point(578, 20);
            this.labelPort.Name = "labelPort";
            this.labelPort.Size = new System.Drawing.Size(74, 24);
            this.labelPort.TabIndex = 0;
            this.labelPort.Text = "포트 :";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(654, 15);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(186, 35);
            this.textBoxPort.TabIndex = 1;
            // 
            // labelPath
            // 
            this.labelPath.AutoSize = true;
            this.labelPath.Location = new System.Drawing.Point(30, 84);
            this.labelPath.Name = "labelPath";
            this.labelPath.Size = new System.Drawing.Size(186, 24);
            this.labelPath.TabIndex = 0;
            this.labelPath.Text = "파일 저장 위치 :";
            // 
            // textBoxPath
            // 
            this.textBoxPath.Location = new System.Drawing.Point(30, 111);
            this.textBoxPath.Name = "textBoxPath";
            this.textBoxPath.Size = new System.Drawing.Size(528, 35);
            this.textBoxPath.TabIndex = 5;
            // 
            // buttonPath
            // 
            this.buttonPath.AutoSize = true;
            this.buttonPath.Location = new System.Drawing.Point(582, 84);
            this.buttonPath.Name = "buttonPath";
            this.buttonPath.Size = new System.Drawing.Size(258, 62);
            this.buttonPath.TabIndex = 2;
            this.buttonPath.Text = "찾아보기";
            this.buttonPath.UseVisualStyleBackColor = true;
            this.buttonPath.Click += new System.EventHandler(this.buttonPath_Click);
            // 
            // buttonStartStop
            // 
            this.buttonStartStop.AutoSize = true;
            this.buttonStartStop.Location = new System.Drawing.Point(286, 174);
            this.buttonStartStop.Name = "buttonStartStop";
            this.buttonStartStop.Size = new System.Drawing.Size(341, 62);
            this.buttonStartStop.TabIndex = 3;
            this.buttonStartStop.Text = "시작";
            this.buttonStartStop.UseVisualStyleBackColor = true;
            this.buttonStartStop.Click += new System.EventHandler(this.buttonStartStop_Click);
            // 
            // textBoxState
            // 
            this.textBoxState.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.textBoxState.Location = new System.Drawing.Point(0, 269);
            this.textBoxState.Multiline = true;
            this.textBoxState.Name = "textBoxState";
            this.textBoxState.ReadOnly = true;
            this.textBoxState.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxState.Size = new System.Drawing.Size(870, 256);
            this.textBoxState.TabIndex = 6;
            // 
            // MyServer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(870, 525);
            this.Controls.Add(this.textBoxState);
            this.Controls.Add(this.buttonStartStop);
            this.Controls.Add(this.buttonPath);
            this.Controls.Add(this.textBoxPath);
            this.Controls.Add(this.labelPath);
            this.Controls.Add(this.textBoxPort);
            this.Controls.Add(this.labelPort);
            this.Controls.Add(this.textBoxIP);
            this.Controls.Add(this.labelIP);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "MyServer";
            this.Text = "Server";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Server_FormClosing);
            this.Load += new System.EventHandler(this.Server_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelIP;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Label labelPort;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label labelPath;
        private System.Windows.Forms.TextBox textBoxPath;
        private System.Windows.Forms.Button buttonPath;
        private System.Windows.Forms.Button buttonStartStop;
        private System.Windows.Forms.TextBox textBoxState;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
    }
}

