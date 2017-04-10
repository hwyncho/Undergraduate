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

namespace MyPacketClient
{
    public partial class MyClient : Form
    {
        public string IP = null;
        public int Port = 0;

        TcpClient Client = null;
        public bool bConnect = false;

        private NetworkStream myNetworkStream = null;
        private FileStream myFileStream = null;
        private byte[] myBuffer = new byte[Constants.PACKET_SIZE];

        private Thread ReceiveThread = null;

        public MyClient()
        {
            InitializeComponent();
        }

        private void Client_Load(object sender, EventArgs e)
        {
            this.listView.View = View.Details;
            this.listView.Columns.Add("이름", 140, HorizontalAlignment.Left);
            this.listView.Columns.Add("크기(Byte)", 140, HorizontalAlignment.Left);

            folderBrowserDialog.SelectedPath = "C:\\Users\\Public";
            this.textBoxPath.Text = folderBrowserDialog.SelectedPath;
        }

        private void buttonPath_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK)
                this.textBoxPath.Text = folderBrowserDialog.SelectedPath;
        }

        private void buttonFile_Click(object sender, EventArgs e)
        {
            openFileDialog.InitialDirectory = "C:\\Users\\Public";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
                this.textBoxFile.Text = openFileDialog.FileName;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (this.textBoxFile.Text == "")
                MessageBox.Show("업로드 할 파일을 선택하세요!");
            else
                Upload();
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            if (this.buttonConnect.Text == "접속")
                Connect();
            else if (this.buttonConnect.Text == "종료")
                Disconnect();
        }

        private void listView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            MyMessage myMessage = new MyMessage();
            myMessage.Type = (int)PacketType.download;
            myMessage.Data = this.listView.FocusedItem.SubItems[0].Text;

            Array.Clear(myBuffer, 0, myBuffer.Length);
            MyPacket.Serialize(myMessage).CopyTo(myBuffer, 0);
            myNetworkStream.Write(myBuffer, 0, myBuffer.Length);
        }
        
        public void Connect()
        {
            Client = new TcpClient();

            try
            {
                IP = this.textBoxIP.Text;
                Port = Convert.ToInt32(this.textBoxPort.Text);

                Client.Connect(IP, Port);
                bConnect = true;

                this.textBoxIP.ReadOnly = true;
                this.textBoxPort.ReadOnly = true;

                this.buttonConnect.ForeColor = Color.Red;
                this.buttonConnect.Text = "종료";

                myNetworkStream = Client.GetStream();

                ReceiveThread = new Thread(new ThreadStart(Receive));
                ReceiveThread.Start();
            }
            catch
            {
                MessageBox.Show("접속에 오류가 발생했습니다!");

                return;
            }

            Array.Clear(myBuffer, 0, myBuffer.Length);
            myNetworkStream.Read(myBuffer, 0, myBuffer.Length);
            MyPacket packet = (MyPacket)MyPacket.Deserialize(myBuffer);
            MyDirectoryInfo myDirectory = (MyDirectoryInfo)MyPacket.Deserialize(myBuffer);

            foreach (var item in myDirectory.Info.GetFiles())
            {
                ListViewItem listItem;
                string[] itemStr = new string[2];

                itemStr.SetValue(item.Name, 0);
                itemStr.SetValue(Convert.ToString(item.Length), 1);

                listItem = new ListViewItem(itemStr);
                this.listView.Items.Add(listItem);
            }

            Array.Clear(myBuffer, 0, myBuffer.Length);
        }

        public void Disconnect()
        {
            if (bConnect == false)
                return;
            else
            {
                MyMessage myMessage = new MyMessage();
                myMessage.Type = (int)PacketType.request;
                myMessage.Data = "Disconnect";

                Array.Clear(myBuffer, 0, myBuffer.Length);
                MyPacket.Serialize(myMessage).CopyTo(myBuffer, 0);
                myNetworkStream.Write(myBuffer, 0, myBuffer.Length);

                bConnect = false;

                Client.Close();

                ReceiveThread.Abort();
                myNetworkStream.Close();
                
                this.textBoxIP.ReadOnly = false;
                this.textBoxPort.ReadOnly = false;

                this.buttonConnect.ForeColor = Color.Black;
                this.buttonConnect.Text = "접속";

                this.listView.Items.Clear();
            }
        }

        public void Receive()
        {
            while (bConnect == true)
            {
                Array.Clear(myBuffer, 0, myBuffer.Length);
                myNetworkStream.Read(myBuffer, 0, myBuffer.Length);
                MyPacket myPacket = (MyPacket)MyPacket.Deserialize(myBuffer);

                switch ((int)myPacket.Type)
                {
                    case (int)PacketType.request:
                        MyMessage myMessage = (MyMessage)MyPacket.Deserialize(myBuffer);
                        if (myMessage.Data == "Stop")
                        {
                            if (MessageBox.Show("서버와의 연결이 끊어졌습니다!") == DialogResult.OK)
                                Disconnect();
                        }
                        break;

                    case (int)PacketType.download:
                        Download();
                        break;

                    default:
                        break;
                }
            }
        }

        public void Download()
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

            this.Invoke(new MethodInvoker(delegate () { this.buttonConnect.Enabled = false; }));

            while (true)
            {
                myFileStream.Write(myFile.Data, 0, myFile.SendSize);

                totalSize += myFile.SendSize;

                this.Invoke(new MethodInvoker(delegate () { progressBar.Value = totalSize / fileSize * 100; }));

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

            if (MessageBox.Show("다운로드를 완료했습니다!") == DialogResult.OK)
            {
                this.Invoke(new MethodInvoker(delegate () { progressBar.Value = 0; }));
                this.Invoke(new MethodInvoker(delegate () { this.buttonConnect.Enabled = true; }));
            }
        }

        public void Upload()
        {
            string filePath = null;
            string[] fileNames = null;
            int readSize = 0;
            int totalSize = 0;

            filePath = this.openFileDialog.FileName;
            fileNames = this.openFileDialog.FileName.Split('\\');
            myFileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);

            MyFile myFile = new MyFile();
            myFile.Type = (int)PacketType.upload;
            myFile.Name = fileNames[fileNames.Length - 1];
            myFile.TotalSize = (int)myFileStream.Length;

            this.Invoke(new MethodInvoker(delegate () { this.buttonConnect.Enabled = false; }));

            while (true)
            {
                if (totalSize != myFileStream.Length)
                {
                    Array.Clear(myFile.Data, 0, myFile.Data.Length);
                    Array.Clear(myBuffer, 0, myBuffer.Length);

                    readSize = myFileStream.Read(myFile.Data, 0, myFile.Data.Length);
                    myFile.SendSize = readSize;

                    MyPacket.Serialize(myFile).CopyTo(myBuffer, 0);
                    myNetworkStream.Write(myBuffer, 0, 1024 * 4);

                    totalSize += readSize;

                    this.Invoke(new MethodInvoker(delegate () { progressBar.Value = totalSize / (int)myFileStream.Length * 100; }));
                }
                else
                    break;
            }

            if (MessageBox.Show("업로드를 완료했습니다!") == DialogResult.OK)
            {
                this.Invoke(new MethodInvoker(delegate () { progressBar.Value = 0; }));
                this.Invoke(new MethodInvoker(delegate () { this.buttonConnect.Enabled = true; }));
            }

            myFileStream.Close();
        }
    }
}