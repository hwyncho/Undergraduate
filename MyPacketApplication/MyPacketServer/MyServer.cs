using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MyPacketLibrary;

namespace MyPacketServer
{
    public partial class MyServer : Form
    {
        public string IP = null;
        public int Port = 8000;

        private TcpListener Server = null;
        public bool bServerRun = false;
        public bool bClientConnect = false;
        
        private NetworkStream myNetworkStream = null;
        private FileStream myFileStream = null;
        private byte[] myBuffer = new byte[Constants.PACKET_SIZE];

        private Thread ServerThread = null;
        private Thread ReceiveThread = null;

        public int totalLength = 0;

        public MyServer()
        {
            InitializeComponent();
        }

        private void Server_Load(object sender, EventArgs e)
        {
            try
            {
                /*
                // 외부 IP 주소 받아오기
                string ds = new WebClient().DownloadString("http://checkip.dyndns.org/");
                string myIP = ds.Substring(ds.IndexOf(":") + 2, ds.LastIndexOf("</body>") - ds.IndexOf(":") - 2);
                */ 

                IPHostEntry hostInfo = Dns.GetHostByName(Dns.GetHostName());

                this.IP = hostInfo.AddressList[0].ToString();
                this.textBoxIP.Text = this.IP;

                this.textBoxState.Text = "서버 상태 : 중지";

                folderBrowserDialog.SelectedPath = "C:\\Users\\Public";
                this.textBoxPath.Text = folderBrowserDialog.SelectedPath;
            }
            catch
            {
                MessageBox.Show("네트워크 연결을 확인하세요!");

                this.Close();
            }
        }

        private void Server_FormClosing(object sender, FormClosingEventArgs e)
        {
            ServerStop();
        }

        private void buttonPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                this.textBoxPath.Text = folderBrowserDialog.SelectedPath;
        }

        private void buttonStartStop_Click(object sender, EventArgs e)
        {
            if (this.buttonStartStop.Text == "시작")
            {
                Server_Start();

                ServerThread = new Thread(new ThreadStart(Server_Run));
                ServerThread.Start();
            }
            else if (this.buttonStartStop.Text == "중지")
                ServerStop();
        }

        public void Server_Start()
        {
            try
            {
                if (this.textBoxPort.Text == "")
                    this.textBoxPort.Text = "8000";
                else
                    Port = Convert.ToInt32(this.textBoxPort.Text);

                Server = new TcpListener(Port);

                Server.Start();
                bServerRun = true;
                
                this.textBoxPort.ReadOnly = true;

                this.buttonStartStop.ForeColor = Color.Red;
                this.buttonStartStop.Text = "중지";

                this.textBoxPath.ReadOnly = true;
                this.buttonPath.Enabled = false;

                this.textBoxState.Text = "서버 상태 : 시작";

                this.textBoxState.AppendText(Environment.NewLine);
                this.textBoxState.AppendText("저장 위치 : " + this.textBoxPath.Text);

                this.textBoxState.AppendText(Environment.NewLine);
                this.textBoxState.AppendText("클라이언트의 접속을 기다립니다...");
            }
            catch
            {
                MessageBox.Show("서버 시작에 오류가 발생했습니다!");

                return;
            }
        }

        public void Server_Run()
        {
            Socket client = Server.AcceptSocket();

            if (client.Connected)
            {
                bClientConnect = true;
                
                this.textBoxState.AppendText(Environment.NewLine);
                this.textBoxState.AppendText("클라이언트가 접속했습니다!");

                myNetworkStream = new NetworkStream(client);

                ReceiveThread = new Thread(new ThreadStart(Receive));
                ReceiveThread.Start();

                MyDirectoryInfo myDirectory = new MyDirectoryInfo(this.folderBrowserDialog.SelectedPath);
                MyPacket.Serialize(myDirectory).CopyTo(myBuffer, 0);
                myNetworkStream.Write(myBuffer, 0, myBuffer.Length);
            }

            while (bServerRun == true)
            {
                ;
            }
        }

        public void ServerStop()
        {
            if (bServerRun == true)
            {
                MyMessage myMessage = new MyMessage();
                myMessage.Type = (int)PacketType.request;
                myMessage.Data = "Stop";

                Array.Clear(myBuffer, 0, myBuffer.Length);
                MyPacket.Serialize(myMessage).CopyTo(myBuffer, 0);
                myNetworkStream.Write(myBuffer, 0, myBuffer.Length);

                bClientConnect = false;
                bServerRun = false;

                Server.Stop();

                if (bClientConnect == true)
                {
                    myNetworkStream.Close();
                    ReceiveThread.Abort();
                }

                ServerThread.Abort();
                
                this.textBoxPort.ReadOnly = false;

                this.buttonStartStop.ForeColor = Color.Black;
                this.buttonStartStop.Text = "시작";

                this.textBoxPath.ReadOnly = false;
                this.buttonPath.Enabled = true;

                this.textBoxState.Text = "서버 상태 : 중지";
            }
            else
                return;
        }

        public void Receive()
        {
            while (bClientConnect == true)
            {
                myNetworkStream.Read(myBuffer, 0, myBuffer.Length);
                MyPacket myPacket = (MyPacket)MyPacket.Deserialize(myBuffer);

                switch ((int)myPacket.Type)
                {
                    case (int)PacketType.request:
                        MyMessage myMessage = (MyMessage)MyPacket.Deserialize(myBuffer);
                        if (myMessage.Data == "Disconnect")
                        {
                            if (MessageBox.Show("클라이언트와의 연결이 끊어졌습니다!") == DialogResult.OK)
                                ServerStop();
                        }
                        break;

                    case (int)PacketType.download:
                        Download();
                        break;

                    case (int)PacketType.upload:
                        Upload();
                        break;

                    default:
                        break;
                }
            }
        }

        public void Download()
        {
            string filePath = null;
            string[] fileNames = null;
            int readSize = 0;
            int totalSize = 0;

            MyMessage myMessage = new MyMessage();
            myMessage = (MyMessage)MyPacket.Deserialize(myBuffer);
            
            filePath = this.folderBrowserDialog.SelectedPath + "\\" + myMessage.Data;

            myFileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);

            fileNames = myFileStream.Name.Split('\\');

            MyFile myFile = new MyFile();
            myFile.Type = (int)PacketType.download;
            myFile.Name = fileNames[fileNames.Length - 1];
            myFile.TotalSize = (int)myFileStream.Length;

            this.Invoke(new MethodInvoker(delegate () { this.buttonStartStop.Enabled = false; }));
            
            while (true)
            {
                if (totalSize != myFileStream.Length)
                {
                    Array.Clear(myFile.Data, 0, myFile.Data.Length);
                    Array.Clear(myBuffer, 0, myBuffer.Length);

                    readSize = myFileStream.Read(myFile.Data, 0, myFile.Data.Length);
                    myFile.SendSize = readSize;
                    
                    MyPacket.Serialize(myFile).CopyTo(myBuffer, 0);
                    myNetworkStream.Write(myBuffer, 0, myBuffer.Length);

                    totalSize += readSize;
                }
                else
                    break;
            }

            myFileStream.Close();

            this.Invoke(new MethodInvoker(delegate () { textBoxState.AppendText(Environment.NewLine); }));
            this.Invoke(new MethodInvoker(delegate () { textBoxState.AppendText("다운로드를 완료했습니다."); }));
            this.Invoke(new MethodInvoker(delegate () { this.buttonStartStop.Enabled = true; }));
        }

        public void Upload()
        {
            string filePath = null;
            string fileName = null;
            int fileSize = 0;
            int totalSize = 0;

            MyFile myFile = (MyFile)MyPacket.Deserialize(myBuffer);
            fileName = myFile.Name;
            fileSize = myFile.TotalSize;

            filePath = this.folderBrowserDialog.SelectedPath + "\\" + fileName;
            myFileStream = new FileStream(filePath, FileMode.Create, FileAccess.Write);

            this.Invoke(new MethodInvoker(delegate () { this.buttonStartStop.Enabled = false; }));

            while (true)
            {
                myFileStream.Write(myFile.Data, 0, myFile.SendSize);

                totalSize += myFile.SendSize;

                if (totalSize != fileSize)
                {
                    Array.Clear(myBuffer, 0, myBuffer.Length);
                    Array.Clear(myFile.Data, 0, myFile.Data.Length);

                    myNetworkStream.Read(myBuffer, 0, myBuffer.Length);
                    myFile = (MyFile)MyPacket.Deserialize(myBuffer);
                }
                else
                    break;
            }

            myFileStream.Close();

            this.Invoke(new MethodInvoker(delegate () { textBoxState.AppendText(Environment.NewLine); }));
            this.Invoke(new MethodInvoker(delegate () { textBoxState.AppendText("업로드를 완료했습니다."); }));
            this.Invoke(new MethodInvoker(delegate () { this.buttonStartStop.Enabled = true; }));
        }
    }
}
