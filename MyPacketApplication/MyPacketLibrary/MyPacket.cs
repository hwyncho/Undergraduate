using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace MyPacketLibrary
{
    public enum PacketType : int
    {
        request = 0,
        download = 1,
        upload = 2
    }
    
    public static class Constants
    {
        public const int PACKET_SIZE = (1024 * 4);      // 패킷 크기 : 4KB
        public const int BLOCK_SIZE = (1024 * 3);       // 블록 크기 : 3KB
    }

    [Serializable]
    public class MyPacket
    {
        public int Type;

        public MyPacket()
        {
            this.Type = -1;
        }

        public static byte[] Serialize(Object o)
        {
            try
            {
                MemoryStream memStream = new MemoryStream(Constants.PACKET_SIZE);
                BinaryFormatter formatter = new BinaryFormatter();

                formatter.Serialize(memStream, o);

                return memStream.ToArray();
            }
            catch
            {
                return null;
            }
        }

        public static Object Deserialize(byte[] data)
        {
            try
            {
                MemoryStream memStream = new MemoryStream(Constants.PACKET_SIZE);
                BinaryFormatter formatter = new BinaryFormatter();

                foreach (byte element in data)
                    memStream.WriteByte(element);

                memStream.Position = 0;
                                
                Object obj = formatter.Deserialize(memStream);

                memStream.Close();

                return obj;
            }
            catch
            {
                return null;
            }
        }
    }

    [Serializable]
    public class MyMessage : MyPacket
    {
        public string Data;

        public MyMessage()
        {
            Data = null;
        }
    }

    [Serializable]
    public class MyDirectoryInfo : MyPacket
    {
        public DirectoryInfo Info;

        public MyDirectoryInfo()
        {
            Info = null;
        }

        public MyDirectoryInfo(string path)
        {
            Info = new DirectoryInfo(path);
        }
    }

    [Serializable]
    public class MyFile : MyPacket
    {
        public string Name;         // 파일의 이름
        public int TotalSize;       // 파일의 총 크기
        public byte[] Data;         // 전송할 데이터
        public int SendSize;        // 전송할 데이터의 크기

        public MyFile()
        {
            Name = null;
            TotalSize = 0;
            Data = new byte[Constants.BLOCK_SIZE];
            SendSize = 0;
        }
    }
}